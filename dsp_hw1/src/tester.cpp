#include "tester.h"

HMMTester::HMMTester(const char* file_path, const int& max_num) {
    HMM* _models = new HMM[max_num];
    const int count = load_models(file_path, _models, max_num);

    models.reserve(count);
    for (int i = 0; i < count; ++i)
        models.push_back(_models + i);
}

HMMTester::HMMTester(HMM* _models, const int& count) {
    models.reserve(count);
    for (int i = 0; i < count; ++i)
        models.push_back(_models + i);
}

void HMMTester::set_data(const char* file_path) {
    data.reserve(2500);
    probability.reserve(2500);
    FILE* fp = open_or_die(file_path, "r");

    char buf[MAX_SEQ];

    while (fscanf(fp, "%s", buf) != EOF) {
        const int seq_len = strlen(buf);

        data.push_back(vector<int>(seq_len));
        probability.push_back(vector<double>(models.size()));

        const int D = data.size() - 1;
        for (int i = 0; i < seq_len; ++i)
            data[D][i] = buf[i] - 'A';
    }
}

void HMMTester::test() {
    for (int m = 0; m < models.size(); ++m)
        for (int d = 0; d < data.size(); ++d)
            update_prob(m, d);
}

void HMMTester::output(const char* file_path) const {
    FILE* fp = open_or_die(file_path, "w");

    for (int d = 0; d < data.size(); ++d) {
        const int max_model = distance(probability[d].begin(), max_element(probability[d].begin(), probability[d].end()));
        const int exp = get_exp(probability[d][max_model]);
        const double prob = probability[d][max_model] / pow(10.0, exp);
        fprintf(fp, "%s %lfe%d\n", models[max_model]->model_name, prob, exp);
    }

    fclose(fp);
}

void HMMTester::update_prob(const int& m, const int& d) {
    // Viterbi
    const int T = data[d].size();
    const int N = models[m]->state_num;
    double delta[T][N];

    for (int i = 0; i < N; ++i)
        delta[0][i] = models[m]->initial[i] * models[m]->observation[data[d][0]][i];

    for (int t = 1; t < T; ++t) {
        for (int j = 0; j < N; ++j) {
            delta[t][j] = 0.0;
            for (int i = 0; i < N; ++i) {
                const double prob = delta[t - 1][i] * models[m]->transition[i][j];
                delta[t][j] = delta[t][j] > prob ? delta[t][j] : prob;
            }
            delta[t][j] *= models[m]->observation[data[d][t]][j];
        }
    }

    probability[d][m] = 0.0;
    for (int i = 0; i < N; ++i)
        probability[d][m] = probability[d][m] > delta[T - 1][i] ? probability[d][m] : delta[T - 1][i];
}