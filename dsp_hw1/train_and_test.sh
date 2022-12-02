#!/bin/bash
RE_NUM='^[0-9]+$'
ITER=1000

if [[ $# -eq 1 && $1 =~ $RE_NUM ]]; then
    ITER=$1
fi

python3 init_models.py
rm -rf accuracy.txt
./bin/train_and_test "$ITER" model_init.txt train_datalist.txt data/test_seq.txt modellist.txt result.txt 5