#include "map.h"

ZhuYinBig5Map::ZhuYinBig5Map(Vocab &_voc) : voc(&_voc){};

void ZhuYinBig5Map::read(FILE *map_file, Ngram *reduce_lm, const LogP &min_prob) {
    File _map_file(map_file);
    char *key, *value, *line;

    // for reduction
    int reduced = 0;

    while ((line = _map_file.getline()) != NULL) {
        key = strtok(line, "\t");
        value = strtok(NULL, "\n");

        if (map.count(key) > 0)
            continue;
        vector<string> values;
        values.reserve(max_value);

        string _value(value);
        stringstream ss(_value);
        string s;

        while (getline(ss, s, ' ')) {
            if (values.size() > 0 && reduce_lm && _value.size() > 2) {  // calculate word prob, if word prob is too low, don't add to choice
                const VocabIndex w_unk = reduce_lm->vocab.getIndex(Vocab_Unknown);
                VocabIndex context[] = {Vocab_None};
                VocabIndex w_idx = reduce_lm->vocab.getIndex(s.c_str());
                w_idx = w_idx == Vocab_None ? w_unk : w_idx;
                if (reduce_lm->wordProb(w_idx, context) < min_prob) {
                    ++reduced;
                    continue;
                }
            }
            values.push_back(s);
        }
        if (values.size() == 0)
            printf("Warning, values with size 0\n");

        map.insert(pair<string, vector<string> >(key, values));
    }

    if (reduce_lm)
        printf("Reduced %d choices\n", reduced);

    vector<string> start({Vocab_SentStart});
    map.insert(pair<string, vector<string> >(Vocab_SentStart, start));

    vector<string> end({Vocab_SentEnd});
    map.insert(pair<string, vector<string> >(Vocab_SentEnd, end));

    vector<string> unk({Vocab_Unknown});
    map.insert(pair<string, vector<string> >(Vocab_Unknown, unk));
}

unsigned int ZhuYinBig5Map::num_entry(const VocabString key) const {
    return map.at(string(key)).size();
}

VocabString ZhuYinBig5Map::get(const VocabString key1, const int &key2) {
    return map.at(string(key1))[key2].c_str();
}