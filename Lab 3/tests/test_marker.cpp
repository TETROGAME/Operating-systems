#include <gtest/gtest.h>
#include "Marker.h"
#include "Array.h"
#include <thread>
#include <chrono>

static void notify_start(std::condition_variable& cv, std::mutex& mtx, bool& ready) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_all();
}

TEST(MarkerUnit, MarksThenCleansUpOnFinish) {
    Array arr(2);
    for (int& v : arr) v = 0;

    std::condition_variable start_cv;
    std::mutex start_mtx;
    bool ready = false;

    Marker m(1, arr, start_cv, start_mtx, ready);

    std::thread t(&Marker::run, &m);

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