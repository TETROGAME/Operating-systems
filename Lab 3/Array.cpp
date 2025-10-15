#include "Array.h"

Array::Array(int size) {
    if (size <= 0) {
        throw std::invalid_argument("Size must be positive integer");
    }
    this->data_ = std::make_shared<int[]>(size);
    this->size_ = size;
}

Array::~Array() = default;

int Array::size() const { return size_; }

int& Array::operator[](int index) {
    if (index >= size_) throw std::out_of_range("Index out of range");
    return data_[index];
}

const int & Array::operator[](int index) const {
    if (index >= size_) throw std::out_of_range("Index out of range");
    return data_[index];
}

int* Array::data() { return data_.get(); }
const int* Array::data() const { return data_.get(); }

int * Array::begin() { return data_.get(); }
int * Array::end() { return data_.get() + size_; }
const int * Array::begin() const { return data_.get(); }
const int * Array::end() const { return data_.get() + size_; }
