#include "../include/ThreadHandler.h"
#include "solution_namespace.h"
#include <iostream>

using namespace solution;

ThreadHandler::ThreadHandler(int thread_count, Array &array)
    : array_(array), thread_count_(thread_count) {}

ThreadHandler::~ThreadHandler() {
    for (auto& t : threads_) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void ThreadHandler::print_array() const {
    cout << "Current array state: ";
    for (const int& elem : array_) {
        cout << elem << ' ';
    }
    cout << '\n';
}

void ThreadHandler::run() {
    for (int i = 0; i < thread_count_; i++) {
        markers_.emplace_back(make_unique<Marker>(i + 1, array_, start_cv_, start_mtx_, ready_to_start_));
        threads_.emplace_back(&Marker::run, markers_[i].get());
    }
    cout << "Starting all marker threads...\n";
    {
        lock_guard<mutex> lock(start_mtx_);
        ready_to_start_ = true;
    }
    start_cv_.notify_all();

    int active_threads = thread_count_;
    while (active_threads > 0) {
        cout << "Main thread is waiting for all markers to be blocked...\n";
        while (true) {
            bool all_blocked = true;
            for (int i = 0; i < thread_count_; ++i) {
                if (markers_[i] && !markers_[i]->is_finished() && !markers_[i]->is_blocked()) {
                    all_blocked = false;
                    break;
                }
            }
            if (all_blocked) break;

        }

        cout << "\nAll threads are blocked.\n";
        print_array();

        int terminate_id = -1;
        while (true) {
            cout << "Enter the ID of the thread to terminate: ";
            cin >> terminate_id;
            bool valid_id = false;
            for(const auto& marker : markers_) {
                if(marker && !marker->is_finished() && marker->get_id() == terminate_id) {
                    valid_id = true;
                    break;
                }
            }
            if (valid_id) break;
            cout << "Invalid ID. Please enter an active thread ID.\n";
        }

        Marker* marker_to_terminate = nullptr;
        for (auto& marker : markers_) {
            if (marker && marker->get_id() == terminate_id) {
                marker_to_terminate = marker.get();
                break;
            }
        }

        if (marker_to_terminate) {
            marker_to_terminate->finish();
            for(auto& t : threads_) {
                 if(t.get_id() == marker_to_terminate->get_thread_native_handle()) {
                     if (t.joinable()) {
                         t.join();
                     }
                     break;
                 }
            }
            int marker_index = -1;
            for(int i = 0; i < markers_.size(); ++i) {
                if(markers_[i] && markers_[i]->get_id() == terminate_id) {
                    marker_index = i;
                    break;
                }
            }
            if(marker_index != -1 && threads_[marker_index].joinable()) {
                threads_[marker_index].join();
            }

            cout << "Thread " << terminate_id << " has finished.\n";
            active_threads--;

            print_array();
        }

        if (active_threads == 0) break;

        cout << "Signaling remaining threads to continue...\n";
        for (auto& marker : markers_) {
            if (marker && !marker->is_finished()) {
                marker->unblock();
            }
        }
    }

    cout << "All marker threads have finished. Main thread is exiting.\n";
}