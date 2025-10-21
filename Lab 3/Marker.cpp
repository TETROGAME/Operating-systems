#include "Marker.h"
#include <iostream>
#include <chrono>

Marker::Marker(int id, Array& array, std::condition_variable& start_cv, std::mutex& start_mtx, bool& ready)
    : id_(id), array_(array), start_cv_(start_cv), start_mtx_(start_mtx), ready_to_start_(ready) {}

void Marker::run() {
    thread_id_ = std::this_thread::get_id();
    {
        std::unique_lock<std::mutex> lock(start_mtx_);
        start_cv_.wait(lock, [this]{ return ready_to_start_; });
    }

    srand(id_);
    int counter = 0;

    while (!finished_) {
        int index = rand() % array_.size();
        bool marked = false;

        {
            std::lock_guard<std::mutex> array_lock(array_.get_mutex());
            if (array_[index] == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                array_[index] = id_;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
                counter++;
                marked = true;
            }
        }

        if (marked) {
            continue;
        }


        std::cout << "\nThread number: " << id_ << '\n'
                  << "Marked " << counter << " elements\n"
                  << "Failed to mark element at " << index << " position\n";

        blocked_ = true;

        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]{ return !blocked_ || finished_; });

        if (finished_) {
            break;
        }
    }

    std::cout << "Thread " << id_ << " is cleaning up...\n";
    {
        std::lock_guard<std::mutex> array_lock(array_.get_mutex());
        for (int& elem : array_) {
            if (elem == id_) {
                elem = 0;
            }
        }
    }
    finished_ = true;
    std::cout << "Thread " << id_ << " finished.\n";
}

void Marker::unblock() {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        blocked_ = false;
    }
    cv_.notify_one();
}

void Marker::finish() {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        finished_ = true;
        blocked_ = false;
    }
    cv_.notify_one();
}

int Marker::get_id() const { return id_; }
bool Marker::is_blocked() const { return blocked_; }
bool Marker::is_finished() const { return finished_; }
std::thread::id Marker::get_thread_native_handle() const { return thread_id_; }