#ifndef LAB_3_ARRAY_H
#define LAB_3_ARRAY_H
#include <memory>

class Array {
    std::shared_ptr<int[]> data_;
    int size_;
    std::mutex mtx_;

public:
    Array();
    explicit Array(int size);
    ~Array();

    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    Array(Array&& other) noexcept;
    Array& operator=(Array&& other) noexcept;

    int size() const;
    int& operator[](int index);
    const int& operator[](int index) const;

    std::mutex& get_mutex();

    int* begin();
    int* end();
    const int* begin() const;
    const int* end() const;
};

#endif //LAB_3_ARRAY_H