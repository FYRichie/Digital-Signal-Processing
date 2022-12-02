#include "utils.h"

int get_exp(const double& num) {
    if (num <= 0.0)
        return 0;
    double n = num;
    int e = 0;
    while (n < 1.0) {
        n *= 10.0;
        --e;
    }
    while (n >= 10.0) {
        n /= 10.0;
        ++e;
    }
    return e;
}

string exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}