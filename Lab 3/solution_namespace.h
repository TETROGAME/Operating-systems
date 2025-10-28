#ifndef LAB_3_SOLUTION_NAMESPACE_H
#define LAB_3_SOLUTION_NAMESPACE_H
#include <condition_variable>
#include <iostream>
#include <vector>
#include <sstream>
namespace solution{
    using std::cin;
    using std::cout;
    using std::istringstream;
    using std::ostringstream;

    using std::vector;
    using std::string;

    using std::invalid_argument;

    using std::unique_ptr;
    using std::shared_ptr;

    using std::thread;
    using std::mutex;
    using std::condition_variable;
    using std::lock_guard;
    using std::atomic;
}
#endif //LAB_3_SOLUTION_NAMESPACE_H