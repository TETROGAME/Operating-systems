#include "include/delete_duplicates.h"
template <typename T>
vector<T> delete_duplicates(const vector<T>& in) {
    vector<T> out;
    out.reserve(in.size());
    std::unordered_set<T> seen;
    for (T v : in) {
        if (!seen.contains(v)) { seen.insert(v); out.push_back(v); }
    }
    return out;
}