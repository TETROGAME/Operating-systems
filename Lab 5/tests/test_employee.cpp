#include "gtest/gtest.h"
#include "../headers/Employee.h"
#include "../headers/solution_namespace.h"

using namespace sln;

TEST(EmployeeModelTest, Initialization) {
    Employee e(10, string("JohnDoe"), 40.5);
    EXPECT_EQ(e.num, 10);
    EXPECT_EQ(e.name, string("JohnDoe"));
    EXPECT_DOUBLE_EQ(e.hours, 40.5);
}

TEST(EmployeeModelTest, DefaultConstructor) {
    Employee e;
    EXPECT_EQ(e.num, 0);
    EXPECT_EQ(e.name, string(""));
    EXPECT_DOUBLE_EQ(e.hours, 0.0);
}