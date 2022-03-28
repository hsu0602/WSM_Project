echo -n "Building stemmer... "

g++ -std=c++11 ../src/SourceFile/stemmer.cpp ../src/SourceFile/PorterStemming.cpp -o stemmer

echo "done"

input_dir="../docs/test/newstest/"
output_dir="../docs/test/newsstemmed/"

./stemmer "$input_dir" "$output_dir"


./stemmer ../docs/StopWords/EnglishStopwords/ ../docs/StopWords/EnglishStopwords-stemmed/
