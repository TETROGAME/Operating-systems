#ifndef LAB_3_MARKER_H
#define LAB_3_MARKER_H
#include <condition_variable>
#include "Array.h"


class Marker {
    int id_;
    Array array_;
    static constexpr int sleepTime = 5;

public:
    std::atomic<bool> blocked_;
    std::atomic<bool> shouldFinish_;
    std::condition_variable cv_;
    std::mutex mtx_;

    Marker(int id, Array& array);

    void run();
};


#endif //LAB_3_MARKER_H