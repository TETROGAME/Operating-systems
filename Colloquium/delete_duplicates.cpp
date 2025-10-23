#include "include/delete_duplicates.h"
#include <unordered_set>
#include <vector>

using std::vector;

template <typename T>
vector<T> delete_duplicates(const vector<T>& in) {
    vector<T> out;
    out.reserve(in.size());
    std::unordered_set<T> seen;
    for (T v : in) {
        if (!seen.contains(v)) {
            seen.insert(v);
            out.push_back(v);
        }
    }
    return out;
}

template vector<int> delete_duplicates<int>(const vector<int>&);