for file in $(find . -name '*.wav');
do
  rm "$file"
done
