#include "Array.h"

#include <mutex>
#include <utility>
Array::Array() : data_(nullptr), size_(0) {}

Array::Array(int size) {
    if (size <= 0) {
        throw std::invalid_argument("Size must be a positive integer");
    }
    this->data_ = std::make_shared<int[]>(size);
    this->size_ = size;
}

Array::~Array() = default;

Array::Array(Array&& other) noexcept
    : data_(std::move(other.data_)), size_(other.size_) {
    other.size_ = 0;
}
Array& Array::operator=(Array&& other) noexcept {
    if (this != &other) {
        std::scoped_lock lock(mtx_, other.mtx_);

        data_ = std::move(other.data_);
        size_ = other.size_;
        other.size_ = 0;
    }
    return *this;
}

int Array::size() const { return size_; }

int& Array::operator[](int index) {
    if (index < 0 || index >= size_) throw std::out_of_range("Index out of range");
    return data_[index];
}

const int& Array::operator[](int index) const {
    if (index < 0 || index >= size_) throw std::out_of_range("Index out of range");
    return data_[index];
}

std::mutex& Array::get_mutex() {
    return mtx_;
}

int* Array::begin() { return data_.get(); }
int* Array::end() { return data_.get() + size_; }
const int* Array::begin() const { return data_.get(); }
const int* Array::end() const { return data_.get() + size_; }