echo -n "Building query input... "

g++ -std=c++11 ../src/SourceFile/query.cpp -o query

echo "done"

./query

file=./stemmer

if [ -f "$file" ]; then
    echo "Stemming query... "
else
    echo -n "Buliding stemmer... "
    g++ -std=c++11 ../src/SourceFile/stemmer.cpp ../src/SourceFile/PorterStemming.cpp -o stemmer
    echo "done"
    echo "Stemming query... "
fi

input_dir="../docs/Query/"
output_dir="../docs/QueryStemmed/"

./stemmer "$input_dir" "$output_dir"

sed -f ../docs/StopWords/EnglishStopwords-stemmed/rm-en-EnglishStopwords-anywhere.sed "$output_dir"query.txt | tr -d "[:punct:]" > "$output_dir"query-stop.txt



