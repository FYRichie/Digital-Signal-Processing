#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include "Ngram.h"
#include "map.h"

using namespace std;

class Viterbi {
   public:
    Viterbi(const int&);

    virtual Viterbi& set_map(FILE*);
    Viterbi& set_lm(FILE*);
    Viterbi& set_text(FILE*);
    Viterbi& set_out(FILE*);
    void write();

    const double MIN_PROB = -1e12;
    const unsigned int max_word_per_line = 20000;

   protected:
    virtual string viterbi(const unsigned int&, const VocabString*);

    Vocab voc;
    ZhuYinBig5Map map;
    Ngram language_model;
    File* text_file;
    FILE* out_file;
};

class BigramViterbi : public Viterbi {
   public:
    BigramViterbi();

   private:
    string viterbi(const unsigned int&, const VocabString*);
    LogP get_log_prob(const VocabString, const VocabString);
};

class TrigramViterbi : public Viterbi {
   public:
    TrigramViterbi();
    TrigramViterbi& set_map(FILE*);

    const LogP REDUCE_PROB = -5.5;

   private:
    string viterbi(const unsigned int&, const VocabString*);
    LogP get_log_prob(const VocabString, const VocabString);
    LogP get_log_prob(const VocabString, const VocabString, const VocabString);
};