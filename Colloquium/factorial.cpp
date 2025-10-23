#include "include/factorial.h"
#include <limits>
using std::vector;

vector<unsigned long long> generate_factorials(std::size_t n) {
    vector<unsigned long long> out;
    out.reserve(n);
    unsigned long long cur = 1;
    for (size_t i = 1; i <= n; ++i) {
        if (cur > std::numeric_limits<unsigned long long>::max() / i) break;
        cur *= static_cast<unsigned long long>(i);
        out.push_back(cur);
    }
    return out;
}