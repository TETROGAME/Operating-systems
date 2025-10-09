//
// Created by User on 09.10.2025.
//

#include "Array.h"

Array::Array(size_t size): data_(std::make_unique<int[]>(size)), size_(size) {}

size_t Array::size() const { return size_; }

int & Array::operator[](size_t index) {
    if (index >= size_) throw std::out_of_range("Index out of range");
    return data_[index];
}

const int & Array::operator[](size_t index) const {
    if (index >= size_) throw std::out_of_range("Index out of range");
    return data_[index];
}

int * Array::data() { return data_.get(); }

const int * Array::data() const { return data_.get(); }
