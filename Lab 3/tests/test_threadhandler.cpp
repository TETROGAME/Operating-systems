#include <gtest/gtest.h>
#include "../include/ThreadHandler.h"
#include "../include/Array.h"
#include <sstream>
#include <iostream>
#include <chrono>

TEST(ThreadHandlerUnit, RunSimulatedUserInput) {
    Array arr(2);
    for (int& v : arr) v = 0;

    std::istringstream input("1 2\n");
    auto* orig_cin = std::cin.rdbuf(input.rdbuf());

    std::ostringstream output;
    auto* orig_cout = std::cout.rdbuf(output.rdbuf());

    ThreadHandler handler(2, arr);

    handler.run();

    std::cin.rdbuf(orig_cin);
    std::cout.rdbuf(orig_cout);
    for (const int& v : arr) {
        EXPECT_EQ(v, 0);
    }

    std::string outstr = output.str();
    EXPECT_NE(outstr.find("All marker threads have finished"), std::string::npos);
}