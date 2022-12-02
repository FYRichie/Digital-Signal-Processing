#! /bin/bash
ORDER=3

VIEW_DIR="view"

function preprocess() {
    FILE=$1
    echo "Processing $FILE..."
    perl separator_big5.pl "$FILE".txt > "$FILE"_sep.txt
    ngram-count -text "$FILE"_sep.txt -order "$ORDER" -write "$FILE"_count.txt
    ngram-count -read "$FILE"_count.txt -lm "$FILE".lm -order "$ORDER" -unk
}

preprocess corpus

mkdir -p "$VIEW_DIR"
iconv -f big5 ZhuYin-Big5.map > "$VIEW_DIR"/ZhuYin-Big5.txt
iconv -f big5 Big5-ZhuYin.map > "$VIEW_DIR"/Big5-ZhuYin.txt
