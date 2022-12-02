#include "viterbi.h"

Viterbi::Viterbi(const int& order) : language_model(voc, order), map(voc) {}

Viterbi& Viterbi::set_lm(FILE* _lm_file) {
    File lm_file(_lm_file);
    language_model.read(lm_file);
    lm_file.close();
    return *this;
}

Viterbi& Viterbi::set_map(FILE* _map_file) {
    map.read(_map_file);
    fclose(_map_file);
    return *this;
}

Viterbi& Viterbi::set_text(FILE* _text_file) {
    text_file = new File(_text_file);
    return *this;
}

Viterbi& Viterbi::set_out(FILE* _out_file) {
    out_file = _out_file;
    return *this;
}

void Viterbi::write() {
    char* line;
    VocabString sentence[max_word_per_line];

    while ((line = text_file->getline()) != NULL) {
        unsigned int num_words = Vocab::parseWords(line, sentence, max_word_per_line);
        if (num_words >= max_word_per_line) {
            fprintf(out_file, "Too many words per sentence: %d\n", num_words);
        } else {
            string ans = viterbi(num_words, sentence);
            fprintf(out_file, "<s> %s</s>\n", ans.c_str());
        }
    }

    text_file->close();
    fclose(out_file);
}

string Viterbi::viterbi(const unsigned int& num_words, const VocabString* sentence) {
    return "Viterbi algorithm not implemented.\n";
}

BigramViterbi::BigramViterbi() : Viterbi(2){};

string BigramViterbi::viterbi(const unsigned int& num_words, const VocabString sentence[]) {
    vector<vector<uint32_t> > state(num_words);
    vector<vector<LogP> > delta(num_words);

    for (int i = 0; i < num_words; ++i) {
        const int n_cand = map.num_entry(sentence[i]);
        state[i].resize(n_cand);
        delta[i].resize(n_cand);
    }

    for (int i = 0; i < delta[0].size(); ++i) {
        VocabString cur_word = map.get(sentence[0], i);
        delta[0][i] = get_log_prob(Vocab_SentStart, cur_word);
        state[0][i] = i;
    }

    LogP prob = MIN_PROB;

    for (int t = 1; t < num_words; ++t) {
        for (int i = 0; i < delta[t].size(); ++i) {
            delta[t][i] = MIN_PROB;
            prob = MIN_PROB;

            VocabString cur_word = map.get(sentence[t], i);
            for (int j = 0; j < delta[t - 1].size(); ++j) {
                VocabString prev_word = map.get(sentence[t - 1], j);
                prob = delta[t - 1][j] + get_log_prob(prev_word, cur_word);
                if (prob > delta[t][i]) {
                    delta[t][i] = prob;
                    state[t][i] = j;
                }
            }
        }
    }

    LogP max_prob = MIN_PROB;
    int max_idx = 0;
    vector<string> out_string(num_words);
    for (int i = 0; i < delta[num_words - 1].size(); ++i) {
        if (delta[num_words - 1][i] > max_prob) {
            max_prob = delta[num_words - 1][i];
            max_idx = i;
        }
    }
    out_string[num_words - 1] = string(map.get(sentence[num_words - 1], max_idx));

    for (int t = num_words - 1; t > 0; --t) {
        max_idx = state[t][max_idx];
        out_string[t - 1] = string(map.get(sentence[t - 1], max_idx));
    }

    string ans = "";
    for (int t = 0; t < num_words; ++t)
        ans += out_string[t] + " ";

    return ans;
}

// return log(P(word2 | word1))
LogP BigramViterbi::get_log_prob(const VocabString word1, const VocabString word2) {
    VocabIndex w1_idx = voc.getIndex(word1);
    VocabIndex w2_idx = voc.getIndex(word2);
    const VocabIndex w_unk = voc.getIndex(Vocab_Unknown);

    w1_idx = w1_idx == Vocab_None ? w_unk : w1_idx;
    w2_idx = w2_idx == Vocab_None ? w_unk : w2_idx;

    VocabIndex context[] = {w1_idx, Vocab_None};
    return language_model.wordProb(w2_idx, context);
}

TrigramViterbi::TrigramViterbi() : Viterbi(3){};

TrigramViterbi& TrigramViterbi::set_map(FILE* _map_file) {
    map.read(_map_file, &language_model, REDUCE_PROB);
    fclose(_map_file);
    return *this;
}

string TrigramViterbi::viterbi(const unsigned int& num_words, const VocabString* sentence) {
    vector<vector<vector<uint32_t> > > state(num_words);
    vector<vector<vector<LogP> > > delta(num_words);

    for (int t = 0; t < num_words; ++t) {
        const int n_cand = map.num_entry(sentence[t]);
        state[t].resize(n_cand);
        delta[t].resize(n_cand);

        const int size = t == 0 ? 1 : state[t - 1].size();
        for (int i = 0; i < n_cand; ++i) {
            state[t][i].resize(size);
            delta[t][i].resize(size);
        }
    }

    LogP prob = MIN_PROB;
    // init uni-gram
    for (int i = 0; i < state[0].size(); ++i) {
        VocabString cur_word = map.get(sentence[0], i);
        state[0][i][0] = i;
        delta[0][i][0] = get_log_prob(Vocab_SentStart, cur_word);
    }
    // init bi-gram
    for (int i = 0; i < state[1].size(); ++i) {
        VocabString cur_word = map.get(sentence[1], i);
        for (int j = 0; j < state[0].size(); ++j) {
            VocabString prev_word = map.get(sentence[0], j);
            delta[1][i][j] = get_log_prob(Vocab_SentStart, prev_word, cur_word) + delta[0][j][0];
            state[1][i][j] = i;
        }
    }
    // t >= 2
    for (int t = 2; t < num_words; ++t) {
        // printf("current: %d, i: %d, j: %d, k: %d\n", t, delta[t].size(), delta[t][0].size(), delta[t - 1][0].size());
        for (int i = 0; i < delta[t].size(); ++i) {
            VocabString cur_word = map.get(sentence[t], i);
            for (int j = 0; j < delta[t][i].size(); ++j) {
                VocabString prev_word = map.get(sentence[t - 1], j);
                delta[t][i][j] = MIN_PROB;
                for (int k = 0; k < delta[t - 1][j].size(); ++k) {
                    VocabString p_prev_word = map.get(sentence[t - 2], k);
                    prob = delta[t - 1][j][k] + get_log_prob(p_prev_word, prev_word, cur_word);
                    if (prob > delta[t][i][j]) {
                        delta[t][i][j] = prob;
                        state[t][i][j] = k;
                    }
                }
            }
        }
    }

    // back tracking
    LogP max_prob = MIN_PROB;
    int max_idx = 0;
    vector<string> out_string(num_words);
    int _i = 0, _j = 0, _k = 0;
    for (int i = 0; i < delta[num_words - 1].size(); ++i) {
        for (int j = 0; j < delta[num_words - 1][i].size(); ++j) {
            if (delta[num_words - 1][i][j] > max_prob) {
                max_prob = delta[num_words - 1][i][j];
                _i = i, _j = j, _k = state[num_words - 1][i][j];
            }
        }
    }

    out_string[num_words - 1] = map.get(sentence[num_words - 1], _i);
    for (int t = num_words - 2; t >= 0; --t) {
        _i = _j, _j = _k, _k = state[t][_i][_j];
        out_string[t] = map.get(sentence[t], _i);
    }

    string ans = "";
    for (int t = 0; t < num_words; ++t)
        ans += out_string[t] + " ";

    return ans;
}

// return log(P(word2 | word1))
LogP TrigramViterbi::get_log_prob(const VocabString word1, const VocabString word2) {
    VocabIndex w1_idx = voc.getIndex(word1);
    VocabIndex w2_idx = voc.getIndex(word2);
    const VocabIndex w_unk = voc.getIndex(Vocab_Unknown);

    w1_idx = w1_idx == Vocab_None ? w_unk : w1_idx;
    w2_idx = w2_idx == Vocab_None ? w_unk : w2_idx;

    VocabIndex context[] = {w1_idx, Vocab_None};
    return language_model.wordProb(w2_idx, context);
}

// return log(P(word3 | word1, word2))
LogP TrigramViterbi::get_log_prob(const VocabString word1, const VocabString word2, const VocabString word3) {
    VocabIndex w1_idx = voc.getIndex(word1);
    VocabIndex w2_idx = voc.getIndex(word2);
    VocabIndex w3_idx = voc.getIndex(word3);
    const VocabIndex w_unk = voc.getIndex(Vocab_Unknown);

    w1_idx = w1_idx == Vocab_None ? w_unk : w1_idx;
    w2_idx = w2_idx == Vocab_None ? w_unk : w2_idx;
    w3_idx = w3_idx == Vocab_None ? w_unk : w3_idx;

    VocabIndex context[] = {w2_idx, w1_idx, Vocab_None};
    return language_model.wordProb(w3_idx, context);
}