//
// Created by User on 30.09.2025.
//

#ifndef HOLDER_H
#define HOLDER_H
#include <vector>
using std::vector;

struct Holder {
    vector<int> array;
    int min_index{};
    int max_index{};
    double average{};
    Holder();
    Holder(const vector<int>& array, int min_index, int max_index, double average);
};



#endif //HOLDER_H
