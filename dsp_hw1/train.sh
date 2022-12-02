#!/bin/bash
RE_NUM='^[0-9]+$'
ITER=100

if [[ $# -eq 1 && $1 =~ $RE_NUM ]]; then
    ITER=$1
fi

for i in {1..5}
do
    ./bin/train "$ITER" model_init.txt data/train_seq_0"$i".txt model_0"$i".txt
done