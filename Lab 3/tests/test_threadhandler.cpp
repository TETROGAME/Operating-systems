#include <gtest/gtest.h>
#include "../include/ThreadHandler.h"
#include "../include/Array.h"
#include <sstream>
#include <iostream>
#include <chrono>

using namespace solution;
TEST(ThreadHandlerUnit, RunSimulatedUserInput) {
    Array arr(2);
    for (int& v : arr) v = 0;

    istringstream input("1 2\n");
    auto* orig_cin = cin.rdbuf(input.rdbuf());

    ostringstream output;
    auto* orig_cout = cout.rdbuf(output.rdbuf());

    ThreadHandler handler(2, arr);

    handler.run();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);
    for (const int& v : arr) {
        EXPECT_EQ(v, 0);
    }

    string outstr = output.str();
    EXPECT_NE(outstr.find("All marker threads have finished"), string::npos);
}