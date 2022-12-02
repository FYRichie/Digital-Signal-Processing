#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"
#include "trainer.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Number of argument is incorrect, the argument should be <iter> <model_init_path> <seq_path> <output_model_path>\n");
        return 0;
    }

    const int n_iter = atoi(argv[1]);
    const char* model_init_path = argv[2];
    const char* seq_path = argv[3];
    const char* output_model_path = argv[4];

    HMM hmm;
    loadHMM(&hmm, model_init_path);

    printf("Reading data from %s\n", seq_path);

    HMMTrainer trainer(&hmm);
    trainer.set_data(seq_path);

    for (int i = 0; i < n_iter; ++i) {
        printf("Training epoch: %d...\n", i + 1);
        trainer.train();
    }

    FILE* output_file = open_or_die(output_model_path, "w");
    dumpHMM(output_file, &hmm);

    printf("Model parameters saved to %s\n", output_model_path);

    return 0;
}