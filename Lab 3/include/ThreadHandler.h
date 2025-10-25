#ifndef LAB_3_THREADHANDLER_H
#define LAB_3_THREADHANDLER_H
#include <vector>
#include <thread>
#include <memory>
#include "Array.h"
#include "Marker.h"

class ThreadHandler {
    Array& array_;
    int thread_count_;
    std::vector<std::thread> threads_;
    std::vector<std::unique_ptr<Marker>> markers_;
    std::condition_variable start_cv_;
    std::mutex start_mtx_;
    bool ready_to_start_ = false;

    void print_array() const;

public:
    ThreadHandler(int thread_count, Array& array);
    ~ThreadHandler();
    void run();
};

#endif //LAB_3_THREADHANDLER_H