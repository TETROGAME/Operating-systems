#include <gtest/gtest.h>
#include "Array.h"
#include "ThreadHandler.h"
#include <sstream>
#include <iostream>

TEST(EndToEnd, MainFlowCompletesViaHandler) {
    std::istringstream input("2\n2\n1 2\n");
    auto* orig_cin = std::cin.rdbuf(input.rdbuf());

    std::ostringstream output;
    auto* orig_cout = std::cout.rdbuf(output.rdbuf());

    int size = 0;
    std::cin >> size;
    ASSERT_GT(size, 0) << "Invalid test input for array size";

    Array array;
    try {
        array = Array(size);
    } catch (const std::invalid_argument& e) {
        std::cin.rdbuf(orig_cin);
        std::cout.rdbuf(orig_cout);
        FAIL() << "Array construction threw: " << e.what();
    }

    for (int& elem : array) elem = 0;

    int thread_count = 0;
    std::cin >> thread_count;
    ASSERT_GT(thread_count, 0) << "Invalid test input for thread count";

    ThreadHandler handler(thread_count, array);
    handler.run();

    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);

    for (const int& v : array) {
        EXPECT_EQ(v, 0);
    }

    const std::string out = output.str();
    EXPECT_NE(out.find("All marker threads have finished"), std::string::npos);
}