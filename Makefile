CC = g++

all:
	$(CC) src/*.cpp -std=c++11 -lmpg123 -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -g -o a

clean:
	rm -f a
