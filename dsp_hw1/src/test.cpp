#include <stdio.h>
#include <stdlib.h>
#include "hmm.h"
#include "tester.h"

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Number of argument is incorrect, the argument should be <models_list_path> <seq_path> <output_result_path>\n");
        return 0;
    }

    const char* models_list_path = argv[1];
    const char* seq_path = argv[2];
    const char* output_result_path = argv[3];

    printf("Loading model from %s\n", models_list_path);
    HMMTester tester(models_list_path);
    printf("Reading data from %s\n", seq_path);
    tester.set_data(seq_path);

    printf("Testing...\n");
    tester.test();

    printf("Writing prediction to %s\n", output_result_path);
    tester.output(output_result_path);

    return 0;
}