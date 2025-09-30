#include "ArrayHandler.h"
#include <windows.h>
#include <vector>
#include <iostream>
using std::vector;
using std::cin;
using std::cout;
static const int findMinMaxSleepTime = 7;
static const int findAverageSleepTime = 12;

namespace ArrayHandler {
    vector<int> getArray() {
        cout << "Input array size: ";
        int n;
        cin >> n;
        if (n <= 0) {
            cout << "Invalid input!\n";
            exit(0);
        }
        cout << "Enter array elements: ";
        vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            cin >> arr[i];
        }
        return arr;
    }

    void setMinMaxToAverage(Holder &holder) {
        holder.array[holder.min_index] = holder.average;
        holder.array[holder.max_index] = holder.average;
    }

    void findMinMax(Holder& holder) {
        if (holder.array.empty()) { return; }
        int max = INT_MIN, min = INT_MAX;
        int max_index = -1, min_index = -1;
        for (int i = 0; i < holder.array.size(); i++) {
            if (holder.array[i] > max) {
                max = holder.array[i];
                max_index = i;
            }
            Sleep(findMinMaxSleepTime);
            if (holder.array[i] < min) {
                min = holder.array[i];
                min_index = i;
            }
            Sleep(findMinMaxSleepTime);
        }
        holder.max_index = max_index;
        holder.min_index = min_index;
    }
    void findAverage(Holder& holder) {
        if (holder.array.empty()) { return; }
        double sum = 0;
        for (const int element : holder.array) {
            sum += element;
            Sleep(findAverageSleepTime);
        }
        holder.average = sum / static_cast<int>(holder.array.size());
    }
}