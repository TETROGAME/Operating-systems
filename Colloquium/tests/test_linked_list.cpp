#include "gtest/gtest.h"
#include "../include/linked_list.h"
#include <vector>

TEST(LinkedListTests, ReverseNonEmptyList) {
    LinkedList<int> list = {1,2,3,4,5};
    auto before = list.to_vector();
    std::vector<int> expected_before = {1,2,3,4,5};
    EXPECT_EQ(before, expected_before);

    list.reverse_recursive();
    auto after = list.to_vector();
    std::vector<int> expected_after = {5,4,3,2,1};
    EXPECT_EQ(after, expected_after);
}

TEST(LinkedListTests, ReverseEmptyThrows) {
    LinkedList<int> empty;
    EXPECT_THROW(empty.reverse_recursive(), std::runtime_error);
}