file1=./en-stemmer
file2=./en-searchsys
file3=./en-query

if [ -f "$file3" ]; then
    echo -n "Building query input... "
else
    echo -n "Building query input... "
    g++ -std=c++2a ../../src/SourceFile/en-query.cpp -o en-query
    
fi
echo "done"

./en-query

if [ -f "$file1" ]; then
    echo "Stemming query... "
else
    echo -n "Buliding stemmer... "
    g++ -std=c++2a ../../src/SourceFile/en-stemmer.cpp ../../src/SourceFile/PorterStemming.cpp -o en-stemmer
    echo "done"
    echo "Stemming query... "
fi

input_dir="../../docs/Query/Query/"
output_dir="../../docs/Query/QueryStemmed/"

./en-stemmer "$input_dir" "$output_dir"

sed -f ../../docs/StopWords/EnglishStopwords-stemmed/rm-en-EnglishStopwords-anywhere.sed "$output_dir"query.txt | tr -d "[:punct:]" > "$output_dir"query-stop.txt

echo -n "Building search system... "

if [ -f "$file2" ]; then
    echo "done"
else
    g++ -std=c++2a ../../src/SourceFile/en-search.cpp -o en-searchsys
    echo "done"
fi

./en-searchsys

