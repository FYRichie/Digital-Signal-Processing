#include "hmm.h"
#include "trainer.h"
#include "tester.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;

#define MAX_DATA_DIR_LENGTH 100

int main(int argc, char* argv[]) {
    if (argc != 8) {
        printf("Number of argument is incorrect, the argument should be <iter> <model_init_path> <train_seqs> <test_seq> <model_list_path> <output_result_path> <max_model_num>\n");
        return 0;
    }

    const int n_iter = atoi(argv[1]);
    const char* model_init_path = argv[2];
    const char* train_seqs_path = argv[3];
    const char* test_seq_path = argv[4];
    const char* model_list_path = argv[5];
    const char* output_result_path = argv[6];
    const int max_model_num = atoi(argv[7]);

    // initialize models
    HMM* models = new HMM[max_model_num];
    const int model_count = load_models(model_list_path, models, max_model_num);

    // set training data
    char train_data_path[max_model_num][MAX_DATA_DIR_LENGTH];
    int train_data_count = 0;
    FILE* train_data_fp = open_or_die(train_seqs_path, "r");
    while (fscanf(train_data_fp, "%s", train_data_path[train_data_count]) > 0)
        ++train_data_count;
    if (train_data_count != model_count) {
        printf("Model count is different to data count, stop\n");
        return 0;
    }

    // initialize trainer
    const int count = model_count;
    vector<HMMTrainer> trainers;
    for (int i = 0; i < model_count; ++i) {
        trainers.push_back(HMMTrainer(models + i));
        trainers[i].set_data(train_data_path[i]);
    }

    // initialize tester
    HMMTester tester(models, count);
    tester.set_data(test_seq_path);

    // training and testing
    double best_acc = 0.0;

    for (int i = 0; i < n_iter; ++i) {
        printf("Iteration %d...\n", i + 1);
        for (int c = 0; c < count; ++c)
            trainers[c].train();

        tester.test();
        tester.output(output_result_path);

        char command[100];
        sprintf(command, "python3 cal_acc.py --name \"iteration %d\" --result_file %s", i + 1, output_result_path);
        const double acc = stod(exec(command));

        if (acc > best_acc) {
            printf("Saving models with accuracy %lf\n", acc);
            dump_models(models, count);
            best_acc = acc;
        }
    }

    return 0;
}