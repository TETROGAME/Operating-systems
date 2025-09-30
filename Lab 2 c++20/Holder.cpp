#include "Holder.h"

Holder::Holder() : min_index(-1), max_index(-1), average(0) {}
Holder::Holder(const vector<int>& array, int min_index, int max_index, double average) {
    this->array = array;
    this->min_index = min_index;
    this->max_index = max_index;
    this->average = average;
}