#! /bin/bash
START=1
END=10
TEST_DIR="test_data"
OUT_DIR="out"
VIEW_DIR="view"
SEP_DIR="sep"

mkdir -p "$OUT_DIR"
mkdir -p "$VIEW_DIR"
mkdir -p "$SEP_DIR"

make
make map

for IDX in $(seq "$START" "$END")
do
    echo "Processing $IDX..."
    perl separator_big5.pl "$TEST_DIR"/"$IDX".txt > "$SEP_DIR"/"$IDX".txt
    time ./mydisambig "$SEP_DIR"/"$IDX".txt ZhuYin-Big5.map corpus.lm "$OUT_DIR"/"$IDX"_bigram.txt
    time ./mydisambig "$SEP_DIR"/"$IDX".txt ZhuYin-Big5.map corpus.lm "$OUT_DIR"/"$IDX"_trigram.txt --tri

    iconv -f big5 "$SEP_DIR"/"$IDX".txt > "$VIEW_DIR"/"$IDX".txt
    iconv -f big5 "$OUT_DIR"/"$IDX"_bigram.txt > "$VIEW_DIR"/"$IDX"_bigram.txt
    iconv -f big5 "$OUT_DIR"/"$IDX"_trigram.txt > "$VIEW_DIR"/"$IDX"_trigram.txt
done