#ifndef LAB_3_ARRAY_H
#define LAB_3_ARRAY_H

#include <memory>

class Array {
    std::shared_ptr<int[]> data_;
    int size_;
public:
    Array(int size);
    ~Array();

    int size() const;
    int& operator[](int index);
    const int& operator[](int index) const;

    int* data();
    const int* data() const;

    int* begin();
    int* end();
    const int* begin() const;
    const int* end() const;
};


#endif //LAB_3_ARRAY_H