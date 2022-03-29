echo -n "Building stemmer... "

g++ -std=c++2a ../../src/SourceFile/en-stemmer.cpp ../../src/SourceFile/PorterStemming.cpp -o en-stemmer

echo "done"

input_dir="../../docs/English/EnglishNews/"
output_dir="../../docs/English/EnglishNewsStemmed/"

./en-stemmer "$input_dir" "$output_dir"


./en-stemmer ../../docs/StopWords/EnglishStopwords/ ../../docs/StopWords/EnglishStopwords-stemmed/
