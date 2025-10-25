#include "gtest/gtest.h"
#include "../include/delete_duplicates.h"
#include <vector>

TEST(DeleteDuplicatesTests, RemovesDuplicatesKeepingOrder) {
    std::vector<int> in = {1,2,3,2,4,1};
    auto out = delete_duplicates(in);
    std::vector<int> expected = {1,2,3,4};
    EXPECT_EQ(out, expected);
}

TEST(DeleteDuplicatesTests, ThrowsOnEmptyInput) {
    std::vector<int> empty;
    EXPECT_THROW(delete_duplicates(empty), std::invalid_argument);
}