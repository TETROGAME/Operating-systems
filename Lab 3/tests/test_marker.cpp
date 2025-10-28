#include <gtest/gtest.h>
#include "../include/Marker.h"
#include "../include/Array.h"
#include <thread>
#include <chrono>

using namespace solution;
static void notify_start(condition_variable& cv, mutex& mtx, bool& ready) {
    {
        lock_guard<mutex> lock(mtx);
        ready = true;
    }
    cv.notify_all();
}

TEST(MarkerUnit, MarksThenCleansUpOnFinish) {
    Array arr(2);
    for (int& v : arr) v = 0;

    condition_variable start_cv;
    mutex start_mtx;
    bool ready = false;

    Marker m(1, arr, start_cv, start_mtx, ready);

    thread t(&Marker::run, &m);

    notify_start(start_cv, start_mtx, ready);

    bool blocked = false;
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (std::chrono::steady_clock::now() < deadline) {
        if (m.is_blocked()) { blocked = true; break; }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    EXPECT_TRUE(blocked) << "Marker did not block within timeout";

    m.finish();

    if (t.joinable()) t.join();

    EXPECT_TRUE(m.is_finished());

    for (const int& v : arr) {
        EXPECT_EQ(v, 0);
    }
}