#ifndef LAB_3_MARKER_H
#define LAB_3_MARKER_H
#include <condition_variable>
#include <atomic>
#include <thread>
#include "Array.h"

class Marker {
    int id_;
    Array& array_;
    static constexpr int sleepTime = 5;

    std::atomic<bool> blocked_{false};
    std::atomic<bool> finished_{false};
    std::condition_variable cv_;
    std::mutex mtx_;

    std::condition_variable& start_cv_;
    std::mutex& start_mtx_;
    bool& ready_to_start_;

    std::thread::id thread_id_;

public:
    Marker(int id, Array& array, std::condition_variable& start_cv, std::mutex& start_mtx, bool& ready);

    void run();
    void unblock();
    void finish();

    int get_id() const;
    bool is_blocked() const;
    bool is_finished() const;
    std::thread::id get_thread_native_handle() const;
};

#endif //LAB_3_MARKER_H