#include "gtest/gtest.h"
#include "../include/factorial.h"
#include <vector>

TEST(FactorialTests, GeneratesFirstFactorials) {
    auto v = generate_factorials(5);

    std::vector<unsigned long long> expected = {1,2,6,24,120};
    ASSERT_EQ(v.size(), expected.size());
    EXPECT_EQ(v, expected);
}

TEST(FactorialTests, ThrowsOnZeroInput) {
    EXPECT_THROW(generate_factorials(0), std::invalid_argument);
}