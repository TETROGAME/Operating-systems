#ifndef LAB_3_THREADHANDLER_H
#define LAB_3_THREADHANDLER_H
#include <vector>
#include <thread>
#include "solution_namespace.h"
#include "Array.h"
#include "Marker.h"

using namespace solution;

class ThreadHandler {
    Array& array_;
    int thread_count_;
    vector<thread> threads_;
    vector<unique_ptr<Marker>> markers_;
    condition_variable start_cv_;
    mutex start_mtx_;
    bool ready_to_start_ = false;

    void print_array() const;

public:
    ThreadHandler(int thread_count, Array& array);
    ~ThreadHandler();
    void run();
};

#endif //LAB_3_THREADHANDLER_H