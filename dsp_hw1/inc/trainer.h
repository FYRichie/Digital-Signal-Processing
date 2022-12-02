#include "hmm.h"
#include <vector>

using namespace std;

class HMMTrainer {
   public:
    HMMTrainer(HMM *);
    void set_data(const char *);
    void train();

   private:
    inline void update_alpha(const int& d);
    inline void update_beta(const int& d);
    inline void update_gamma(const int& d);
    inline void update_epsilon(const int& d);
    inline void update_parameters();

    HMM *hmm;
    vector<vector<double *>> alpha;     // D * T * N
    vector<vector<double *>> beta;      // D * T * N
    vector<vector<double *>> gamma;     // D * T * N
    vector<vector<double **>> epsilon;  // D * (T - 1) * N * N
    vector<vector<int>> data;           // D * T

    int n_data;
};