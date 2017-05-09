#include "SoundFileReaderMp3.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>
#include <bitset>

// Helper function
std::string printBufferVerbose(unsigned char* buffer, std::size_t numBytes){
	std::stringstream stream;
	stream << "print buffer ========================="  << std::endl;
	std::size_t i = 0;
	for (; i < numBytes; i++) {
		if(i % 4 == 0)
			stream << std::endl << i;
		unsigned char word = buffer[i];
		std::bitset<8> bits{word};
		stream << "\t" << bits;
	}

	if(numBytes)
		stream << std::endl << std::endl << "====================================== the first " << i << " bytes" << std::endl;
	return stream.str();
}

// Helper function
std::string printBuffer(unsigned char* buffer, std::size_t numBytes){
	std::stringstream stream;
	stream << "first " << numBytes << " bytes:";
	if(numBytes) {
		std::size_t i = 0;
		for (; i < numBytes; i++) {
			unsigned char word = buffer[i];
			std::bitset<8> bits{word};
			stream << " " << bits;
		}
	} else {
		stream << " buffer empty";
	}
	return stream.str();
}

namespace audio {

SoundFileReaderMp3::SoundFileReaderMp3()
	: m_handle            (nullptr)
	, m_stream            (nullptr)
	, m_frameInfo         ()
	, m_outputFormat      ()
	, m_streambufferSize  (80000) // any size will work
	, m_streambuffer      (new unsigned char[m_streambufferSize])
{}

SoundFileReaderMp3::~SoundFileReaderMp3()
{
	close();
}

void SoundFileReaderMp3::close() {
	if(m_handle) {
		mpg123_close(m_handle);
		mpg123_delete(m_handle);
		mpg123_exit();
		m_handle = nullptr;
	}
    if (m_streambuffer) {
        delete [] m_streambuffer;
        m_streambuffer = nullptr;
    }
}

// \brief Open a sound file for reading
// \param stream Source stream to read from
// \param info   Structure to fill with the properties of the loaded sound
// \return True if the file was successfully opened
bool SoundFileReaderMp3::open(sf::InputStream& stream, sf::SoundFileReader::Info& info) {
	initializeLibrary();
	probeFirstFrame(stream);

	 if(!m_outputFormat.update(m_handle)){
		return false;
	}

	if(!m_frameInfo.update(m_handle)){
		return false;
	}

	fillSfmlInfo(info);

	// save the reference since we never get it again
	m_stream = &stream;

	return true;
}

// calls mpg123_init(), mpg123_new() and mpg123_open_feed
// and does error handling
bool SoundFileReaderMp3::initializeLibrary() {
	if(m_handle) {
		std::cerr << "library already initialized" << std::endl;
		return false;
	}
	// Initialise the mpg123 library.
	// This function is not thread-safe. Call it exactly once per process,
	// before any other (possibly threaded) work with the library.
	int  ok = mpg123_init();
	if (ok != MPG123_OK) {
		std::cerr << "mpg123_init(): " << mpg123_plain_strerror(ok) << std::endl;
		return false;
	}

	// create handle with optional choice of decoder
	m_handle = mpg123_new(NULL, &ok);
	if (!m_handle) {
		std::cerr << "Unable to create mpg123 handle: " << mpg123_plain_strerror(ok) << std::endl;
		return false;
	}

	// Later we have to read the audio input from sf::InputStream, which can be from a file,
	// from the network or from any other input stream. This decides how we open mpg123:
	ok = mpg123_open_feed(m_handle);
	if (ok != MPG123_OK) {
		std::cerr << "mpg123_open_feed(): " << mpg123_plain_strerror(ok) << std::endl;
		return false;
	}
	return true;
}

bool SoundFileReaderMp3::probeFirstFrame(sf::InputStream& stream) {
	// select a rather small buffer size. we don't want to read actual payload data.
	// we only want to read till the mp3 library can extract header information
	std::size_t streamSize =  2048;
	unsigned char streamBuffer[streamSize];

	std::size_t totalReadBytes = 0;
	std::size_t totalDecodedBytes = 0;
	std::size_t numReadsFromStream = 0;

	for(bool needMore = true; needMore;) {
		sf::Int64 bytesReadFromStream = stream.read(streamBuffer, streamSize); // read from input stream
		numReadsFromStream++;
		if (bytesReadFromStream <= 0) {
			break;
		}
		totalReadBytes += bytesReadFromStream;

		// feed until happy, do not touch decoded data. we only want to get header information
		std::size_t numDecodedBytes = 0;
		int ret = mpg123_decode(m_handle, streamBuffer, bytesReadFromStream, nullptr, 0, &numDecodedBytes);
		totalDecodedBytes += numDecodedBytes;

		needMore = ret == MPG123_NEED_MORE;
	}
	std::cout << "";
}

void SoundFileReaderMp3::fillSfmlInfo(sf::SoundFileReader::Info& info) const {
	info.channelCount = m_frameInfo.getNumberOfChannels();
	info.sampleCount = m_frameInfo.getExpectedNumberOfSamples();
	info.sampleRate = m_frameInfo.getSamplingRateInHz();
}

// \brief Read audio samples from the open file
// \param samples  Pointer to the sample array to fill
// \param maxCount Maximum number of samples to read
// \return Number of samples actually read. If less samples than maxCount are read, sfml assumes that EOF has been reached and terminates playback.
sf::Uint64 SoundFileReaderMp3::read(sf::Int16* samples, sf::Uint64 maxCount) {
	// since we get fed bytes from the input stream, casting the
	// output buffer to bytes makes life easier
	unsigned char* byteSamples = (unsigned char*) samples;
	// adjust the requested length
	std::size_t maxBytes = maxCount * sizeof(short);
	std::size_t offset = 0;

	std::size_t totalReadBytes = 0;

	// README: How the sf Audio playback loop works:
	// sf::SoundFileReader::read() gets called in a loop.
	// sf::Soundstream breaks the loop if the return value is 0
	// or smaller than the requested number of samples, maxCount.

	// This is not the main decoding loop but a clean-up loop that consumes any input
	// left from the previous call. This happens if the sample buffer was almost full
	// but the decoder needed more raw input. We read from the stream again and feed
	// everything to the decoder. The decoder is only allowed to fill the small amount
	// left in the sample buffer and has to keep the remaining input.
	// Now is the time to consume that remaining input.
	int ret = 0;
	do
	{
		std::size_t numDecodedBytes = 0;
		ret = mpg123_decode(m_handle,
				  nullptr      // raw input buffer: since we want to consume the remaining input, we don't provide any new one
				, 0            // length of input:  same reason
				, &byteSamples[offset] // array of decoded bytes at sample offset
			    , maxBytes-offset      // number of decoded bytes we want to get out: this calculation is why casting to byte size was a good idea
				, &numDecodedBytes);         // this tells us the size of the decoded data. it will never exceed the size we requested in the above parameter
		offset += numDecodedBytes;     // set sample offset for next call

	} // NEED_MORE is returned when no more input is available and the output array is not full yet. not sure about ERR though. probably when output is full.
	while (ret != MPG123_NEED_MORE && ret != MPG123_ERR && offset < maxBytes);

	while (ret == MPG123_NEED_MORE && ret != MPG123_ERR && offset < maxBytes)
	{
		// now run the normal reading-decoding loop
		sf::Int64 bytesReadFromStream = m_stream->read(m_streambuffer, m_streambufferSize);
		if (bytesReadFromStream <= 0) {
			break;
		}
		totalReadBytes += bytesReadFromStream;

		// feed the input to the decoder
		std::size_t numDecodedBytes = 0;
		ret = mpg123_decode(m_handle, m_streambuffer, bytesReadFromStream, &byteSamples[offset], maxBytes-offset, &numDecodedBytes);
		offset += numDecodedBytes;
	}

	std::size_t totalSamples = offset/sizeof(short);

	return totalSamples;
}

// \brief Change the current read position to the given sample offset
// If the given offset exceeds the total number of samples,
// this function must jump to the end of the file.
// \param sampleOffset Index of the sample to jump to, relative to the beginning
void SoundFileReaderMp3::seek(sf::Uint64 sampleOffset) {
	// probably should go something like this
//	off_t ok = mpg123_feedseek (m_handle, sampleOffset, SEEK_SET, NULL);
}

// Quick check if the reader can handle the input
bool SoundFileReaderMp3::check(sf::InputStream& stream) {
	SoundFileReaderMp3 reader;
	bool result = reader.initializeLibrary();
	if(result)
		result = reader.probeFirstFrame(stream);
	if(result)
		result = reader.m_outputFormat.update(reader.m_handle);
	if(result)
		result = reader.m_frameInfo.update(reader.m_handle);

	// no cleanup required since destructor of reader takes care of everything
	return result;
}

} // namespace audio

