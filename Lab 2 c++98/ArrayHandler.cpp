#include "ArrayHandler.h"
#include <windows.h>
#include <vector>
#include <iostream>
using std::vector;
using std::cin;
using std::cout;
static const int findMinMaxSleepTime = 7;
static const int findAverageSleepTime = 12;

Holder::Holder() : min_index(-1), max_index(-1), average(0) {}
Holder::Holder(const vector<int>& array, int min_index, int max_index, double average) {
    this->array = array;
    this->min_index = min_index;
    this->max_index = max_index;
    this->average = average;
}
vector<int> ArrayHandler::getArray() {
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

void ArrayHandler::setMinMaxToAverage(Holder &holder) {
    holder.array[holder.min_index] = holder.average;
    holder.array[holder.max_index] = holder.average;
}

DWORD WINAPI ArrayHandler::findMinMax(LPVOID lpParameter) {
    Holder* holder = static_cast<Holder*>(lpParameter);
    if (holder->array.empty()) {
        return 0;
    }
    int max = INT_MIN, min = INT_MAX;
    int max_index = -1, min_index = -1;
    for (int i = 0; i < holder->array.size(); i++) {
        if (holder->array[i] > max) {
            max = holder->array[i];
            max_index = i;
        }
        Sleep(findMinMaxSleepTime);
        if (holder->array[i] < min) {
            min = holder->array[i];
            min_index = i;
        }
        Sleep(findMinMaxSleepTime);
    }
    holder->max_index = max_index;
    holder->min_index = min_index;
    return 0;
}
DWORD WINAPI ArrayHandler::findAverage(LPVOID lpParameter) {
    Holder* holder = static_cast<Holder*>(lpParameter);
    if (holder->array.empty()) {
        return 0;
    }
    double sum = 0;
    for (int i = 0; i < holder->array.size(); i++) {
        sum += holder->array[i];
        Sleep(findAverageSleepTime);
    }
    holder->average = sum / static_cast<int>(holder->array.size());
    return 0;
}