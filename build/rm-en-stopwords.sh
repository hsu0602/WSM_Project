echo -n "Building stop word list... "

sed 's/.*/s|\\\<&\\\>||g/' ../docs/StopWords/EnglishStopwords-stemmed/EnglishStopwords.txt > ../docs/StopWords/EnglishStopwords-stemmed/rm-en-EnglishStopwords-anywhere.sed

echo "done"

input_dir="../docs/test/newsstemmed/"
output_dir="../docs/test/newsstemmed-stop/"

input_files=`ls $input_dir | grep .txt`

echo "Removing stop words from $input_dir to $output_dir"

for eachfile in $input_files
do
    echo -n "Removing stop words in $eachfile... "
    sed -f ../docs/StopWords/EnglishStopwords-stemmed/rm-en-EnglishStopwords-anywhere.sed "$input_dir$eachfile" | tr -d "[:punct:]" > "$output_dir$eachfile"
    echo "done"
done
