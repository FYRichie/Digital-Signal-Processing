# Digital-Signal-Processing

## Homework 1

### Implementing Discrete Hidden Markov Model

If you wish to run the code, type the following commands in shell:
```bash
make                    # Compile the cpp files
bash train.sh           # Train the HMM models
bash test.sh            # Test the HMM models
bash train_and_test.sh  # Train and test the hmm models, and output an accuracy file

make clean              # Clean the object and binary files
```

## Homework 2-1

### Automatic Speech Recognition of Mandarin Digits

## Homework 3

### ZhuYin Decoding

First setup the environment:
```bash
docker run -it -v $(pwd)/dsp_hw3:/home/hw3 ntudsp2020autumn/srilm bash
```
In the docker container, type the following commands in shell:
```bash
cd /home/hw3
source setup.sh
bash preprocess.sh                        # Train the language model on corpus.txt
bash run.sh                               # First, map Big5-ZhuYin to ZhuYin-Big5. Second, compile the cpp files. Finally, test the bigram and trigram viterbi algorithms on files in test_data/

# Files in out/ is the origin output (without big5 encoded) of the disambig/mydisambig
# Files in sep/ is the separated files of the test data
# Files in view/ is the big5 encoded files of the origin output of disambig/mydisambig

bash clean.sh                             # Removes the object and binary files, view/, out/, sep/
rm -rf corpus.lm corpus_* ZhuYin-Big5.map # Removes the language model files generated by ncount
```