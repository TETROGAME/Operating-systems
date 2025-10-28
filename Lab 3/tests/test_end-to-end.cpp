#include <gtest/gtest.h>
#include "../include/Array.h"
#include "../include/ThreadHandler.h"
#include "solution_namespace.h"
#include <sstream>
#include <iostream>
using namespace solution;
TEST(EndToEnd, MainFlowCompletesViaHandler) {
    istringstream input("2\n2\n1 2\n");
    auto* orig_cin = cin.rdbuf(input.rdbuf());

    ostringstream output;
    auto* orig_cout = cout.rdbuf(output.rdbuf());

    int size = 0;
    cin >> size;
    ASSERT_GT(size, 0) << "Invalid test input for array size";

    Array array;
    try {
        array = Array(size);
    } catch (const invalid_argument& e) {
        cin.rdbuf(orig_cin);
        cout.rdbuf(orig_cout);
        FAIL() << "Array construction threw: " << e.what();
    }

    for (int& elem : array) elem = 0;

    int thread_count = 0;
    cin >> thread_count;
    ASSERT_GT(thread_count, 0) << "Invalid test input for thread count";

    ThreadHandler handler(thread_count, array);
    handler.run();

    cin.rdbuf(orig_cin);
    cout.rdbuf(orig_cout);

    for (const int& v : array) {
        EXPECT_EQ(v, 0);
    }

    const string out = output.str();
    EXPECT_NE(out.find("All marker threads have finished"), string::npos);
}