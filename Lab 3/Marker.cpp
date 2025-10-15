#include "Marker.h"
#include <iostream>
#include <chrono>
Marker::Marker(int id, Array &array): id_(id), array_(array), blocked_(false), shouldFinish_(false) {}

void Marker::run() {
    srand(id_);
    int counter = 0;
    while (true) {
        int index = rand() % array_.size();
        if (array_[index] == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            array_[index] = id_;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            counter++;
        }
        else {
            std::cout << "\nThread number : " << id_ << '\n';
            std::cout << "Marked " << counter << " elements\n";
            std::cout << "Failed to mark element at " << index << " position\n";
            blocked_ = true;

            std::unique_lock<std::mutex> unique_lock(mtx_);
            cv_.wait(unique_lock, [this]{ return shouldFinish_ || !blocked_; });

            if (shouldFinish_) {
                for (int& elem : array_) {
                    if (elem == id_) elem = 0;
                }
                blocked_ = false;
                std::cout << "Thread " << id_ << " finished\n";
                break;
            }
        }
    }
}
