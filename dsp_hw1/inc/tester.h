#include "hmm.h"
#include "utils.h"
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

class HMMTester {
   public:
    HMMTester(const char*, const int& = 5);
    HMMTester(HMM*, const int&);
    void set_data(const char*);
    void test();
    void output(const char*) const;

   private:
    void update_prob(const int&, const int&);

    vector<HMM*> models;                 // M
    vector<vector<int>> data;            // D * T
    vector<vector<double>> probability;  // D * M
};