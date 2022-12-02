#include <stdio.h>
#include <string.h>
#include "viterbi.h"

using namespace std;

int err;

int main(int argc, char* argv[]) {
    const char* text_file_dir = argv[1];
    const char* map_file_dir = argv[2];
    const char* lm_file_dir = argv[3];
    const char* out_file_dir = argv[4];
    const bool tri_gram = (argc == 6 && strcmp(argv[5], "--tri") == 0);

    FILE* text_file = fopen(text_file_dir, "r");
    FILE* map_file = fopen(map_file_dir, "r");
    FILE* lm_file = fopen(lm_file_dir, "r");
    FILE* out_file = fopen(out_file_dir, "w");

    Viterbi* viterbi = NULL;
    if (tri_gram)
        viterbi = new TrigramViterbi;
    else
        viterbi = new BigramViterbi;

    viterbi->set_lm(lm_file)
        .set_map(map_file)
        .set_text(text_file)
        .set_out(out_file);
    viterbi->write();

    return 0;
}