#include <gtest/gtest.h>
#include "Array.h"
#include <algorithm>

TEST(ArrayUnit, ConstructorAndSize) {
    Array a(5);
    EXPECT_EQ(a.size(), 5);
}

TEST(ArrayUnit, IndexAccessAndBounds) {
    Array a(3);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    EXPECT_EQ(a[2], 30);

    EXPECT_THROW(a[-1], std::out_of_range);
    EXPECT_THROW(a[3], std::out_of_range);
}

TEST(ArrayUnit, Iteration) {
    Array a(4);
    for (int& v : a) { v = 7; }
    for (const int& v : a) {
        EXPECT_EQ(v, 7);
    }
}

TEST(ArrayUnit, MoveConstructorAndAssignment) {
    Array a(2);
    a[0] = 42;
    a[1] = 99;

    Array b(std::move(a));
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], 42);
    EXPECT_EQ(b[1], 99);
    EXPECT_EQ(a.size(), 0);

    Array c(1);
    c = std::move(b);
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c[0], 42);
    EXPECT_EQ(c[1], 99);
    EXPECT_EQ(b.size(), 0);
}