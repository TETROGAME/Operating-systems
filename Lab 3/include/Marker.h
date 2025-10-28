#ifndef LAB_3_MARKER_H
#define LAB_3_MARKER_H
#include <condition_variable>
#include <atomic>
#include <thread>
#include "Array.h"
#include "solution_namespace.h"

using namespace solution;

class Marker {
    int id_;
    Array& array_;
    static constexpr int sleepTime = 5;

    atomic<bool> blocked_{false};
    atomic<bool> finished_{false};
    condition_variable cv_;
    mutex mtx_;

    condition_variable& start_cv_;
    mutex& start_mtx_;
    bool& ready_to_start_;

    thread::id thread_id_;

public:
    Marker(int id, Array& array, condition_variable& start_cv, mutex& start_mtx, bool& ready);

    void run();
    void unblock();
    void finish();

    int get_id() const;
    bool is_blocked() const;
    bool is_finished() const;
    thread::id get_thread_native_handle() const;
};

#endif //LAB_3_MARKER_H