#include "trainer.h"

HMMTrainer::HMMTrainer(HMM* _hmm) : hmm(_hmm) {}

void HMMTrainer::set_data(const char* file_path) {
    data.reserve(10000);
    alpha.reserve(10000);
    beta.reserve(10000);
    gamma.reserve(10000);
    FILE* fp = open_or_die(file_path, "r");

    char buf[MAX_SEQ];

    while (fscanf(fp, "%s", buf) != EOF) {
        const int seq_len = strlen(buf);

        data.push_back(vector<int>(seq_len));
        alpha.push_back(vector<double*>(seq_len));
        beta.push_back(vector<double*>(seq_len));
        gamma.push_back(vector<double*>(seq_len));
        epsilon.push_back(vector<double**>(seq_len - 1));

        const int D = data.size() - 1;
        for (int i = 0; i < seq_len; ++i) {
            data[D][i] = buf[i] - 'A';
            alpha[D][i] = new double[hmm->state_num];
            beta[D][i] = new double[hmm->state_num];
            gamma[D][i] = new double[hmm->state_num];
            if (i < seq_len - 1) {
                epsilon[D][i] = new double*[hmm->state_num];
                for (int j = 0; j < hmm->state_num; ++j)
                    epsilon[D][i][j] = new double[hmm->state_num];
            }
        }
    }
}

void HMMTrainer::update_alpha(const int& d) {
    // t = 0
    for (int i = 0; i < hmm->state_num; ++i)
        alpha[d][0][i] = hmm->initial[i] * hmm->observation[data[d][0]][i];
    // t = 1 ~ T - 1
    for (int t = 1; t < data[d].size(); ++t) {
        for (int j = 0; j < hmm->state_num; ++j) {
            double sum = 0.0;
            for (int i = 0; i < hmm->state_num; ++i)
                sum += alpha[d][t - 1][i] * hmm->transition[i][j];
            alpha[d][t][j] = sum * hmm->observation[data[d][t]][j];
        }
    }
}

void HMMTrainer::update_beta(const int& d) {
    // t = T - 1
    const int T = data[d].size() - 1;
    for (int i = 0; i < hmm->state_num; ++i)
        beta[d][T][i] = 1.0;
    // t = T - 2 ~ 0
    for (int t = T - 1; t >= 0; --t) {
        for (int i = 0; i < hmm->state_num; ++i) {
            double sum = 0.0;
            for (int j = 0; j < hmm->state_num; ++j)
                sum += hmm->transition[i][j] * hmm->observation[data[d][t + 1]][j] * beta[d][t + 1][j];
            beta[d][t][i] = sum;
        }
    }
}

void HMMTrainer::update_gamma(const int& d) {
    for (int t = 0; t < data[d].size(); ++t) {
        double sum = 0.0;
        for (int j = 0; j < hmm->state_num; ++j) {
            gamma[d][t][j] = alpha[d][t][j] * beta[d][t][j];
            sum += gamma[d][t][j];
        }
        for (int i = 0; i < hmm->state_num; ++i)
            gamma[d][t][i] /= sum;
    }
}

void HMMTrainer::update_epsilon(const int& d) {
    for (int t = 0; t < data[d].size() - 1; ++t) {
        double sum = 0.0;
        for (int i = 0; i < hmm->state_num; ++i){
            for (int j = 0; j < hmm->state_num; ++j) {
                epsilon[d][t][i][j] = alpha[d][t][i] * hmm->transition[i][j] * hmm->observation[data[d][t + 1]][j] * beta[d][t + 1][j];
                sum += epsilon[d][t][i][j];
            }
        }
        for (int i = 0; i < hmm->state_num; ++i)
            for (int j = 0; j < hmm->state_num; ++j)
                epsilon[d][t][i][j] /= sum;
    }
}

void HMMTrainer::update_parameters() {
    const int D = data.size();

    // initial
    for (int i = 0; i < hmm->state_num; ++i) {
        double sum_gamma = 0.0;
        for (int d = 0; d < D; ++d)
            sum_gamma += gamma[d][0][i];
        hmm->initial[i] = sum_gamma / D;
    }

    // transition
    for (int i = 0; i < hmm->state_num; ++i) {
        double sum_gamma = 0.0;
        for (int d = 0; d < D; ++d)
            for (int t = 0; t < data[d].size() - 1; ++t)
                sum_gamma += gamma[d][t][i];

        for (int j = 0; j < hmm->state_num; ++j) {
            double sum_epsilon = 0.0;

            for (int d = 0; d < D; ++d)
                for (int t = 0; t < data[d].size() - 1; ++t)
                    sum_epsilon += epsilon[d][t][i][j];

            hmm->transition[i][j] = sum_epsilon / sum_gamma;
        }
    }

    // observation
    for (int i = 0; i < hmm->state_num; ++i) {
        double sum_gamma = 0.0;
        for (int o = 0; o < hmm->observ_num; ++o)
            hmm->observation[o][i] = 0.0;
        for (int d = 0; d < D; ++d) {
            for (int t = 0; t < data[d].size(); ++t) {
                sum_gamma += gamma[d][t][i];
                hmm->observation[data[d][t]][i] += gamma[d][t][i];
            }
        }
        for (int o = 0; o < hmm->observ_num; ++o)
            hmm->observation[o][i] /= sum_gamma;
    }
}

void HMMTrainer::train() {
    for (int d = 0; d < data.size(); ++d) {
        update_alpha(d);
        update_beta(d);
        update_gamma(d);
        update_epsilon(d);
    }
    update_parameters();
}