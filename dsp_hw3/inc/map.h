#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Ngram.h"

using namespace std;

class ZhuYinBig5Map {
   public:
    ZhuYinBig5Map(Vocab& _voc);
    void read(FILE*, Ngram* reduce_lm = NULL, const LogP& min_prob = -1e12);

    unsigned int num_entry(const VocabString) const;
    VocabString get(const VocabString, const int&);

    const int max_value = 2000;

   private:
    unordered_map<string, vector<string> > map;
    Vocab* voc;
};