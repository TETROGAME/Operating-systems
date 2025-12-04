#include "include/factorial.h"
#include <limits>
#include <stdexcept>

using std::vector;

vector<unsigned long long> generate_factorials(std::size_t n) {
    if (n == 0) {
        throw std::invalid_argument("generate_factorials: n must be > 0");
    }

    vector<unsigned long long> out;
    out.reserve(n);
    unsigned long long cur = 1;
    for (size_t i = 1; i <= n; ++i) {
        if (cur > std::numeric_limits<unsigned long long>::max() / i) {
            throw std::overflow_error("generate_factorials: overflow would occur at " + std::to_string(i) + "!");
        }
        cur *= i;
        out.push_back(cur);
    }
    return out;
}