#include "gtest/gtest.h"
#include "../headers/FileHandler.h"
#include "../headers/solution_namespace.h"

using namespace sln;

class FileOperationsTest : public ::testing::Test {
protected:
    string test_file;
    void SetUp() {
        test_file = string("test_gtest_io.bin");
    }
    void TearDown() {
        remove(test_file.c_str());
    }
};

TEST_F(FileOperationsTest, CreateAndRetrieve) {
    FileHandler fh(test_file);
    vector<Employee> data;
    data.push_back(Employee(1, string("Alpha"), 10.0));
    data.push_back(Employee(2, string("Beta"), 20.0));
    fh.CreateFileWithData(data);
    Employee out;
    ASSERT_TRUE(fh.GetEmployee(1, out));
    EXPECT_EQ(out.num, 1);
    EXPECT_EQ(out.name, string("Alpha"));
    EXPECT_DOUBLE_EQ(out.hours, 10.0);
    EXPECT_FALSE(fh.GetEmployee(99, out));
}

TEST_F(FileOperationsTest, ModifyEmployee) {
    FileHandler fh(test_file);
    vector<Employee> data;
    data.push_back(Employee(5, string("OldName"), 5.0));
    fh.CreateFileWithData(data);
    Employee update(5, string("NewName"), 99.9);
    ASSERT_TRUE(fh.UpdateEmployee(update));
    Employee check;
    ASSERT_TRUE(fh.GetEmployee(5, check));
    EXPECT_EQ(check.name, string("NewName"));
    EXPECT_DOUBLE_EQ(check.hours, 99.9);
}