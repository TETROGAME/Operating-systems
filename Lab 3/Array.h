//
// Created by User on 09.10.2025.
//

#ifndef LAB_3_ARRAY_H
#define LAB_3_ARRAY_H


#include <memory>
#include <stdexcept>

class Array {
    std::unique_ptr<int[]> data_;
    size_t size_;
public:
    Array(size_t size);

    size_t size() const;

    int& operator[](size_t index);

    const int& operator[](size_t index) const;

    int* data();

    const int* data() const;
};


#endif //LAB_3_ARRAY_H