#include "ArrayHandler.h"
#include <windows.h>
#include <vector>
#include <iostream>
using std::vector;
using std::cin;
using std::cout;

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
DWORD WINAPI ArrayHandler::findMinMax(LPVOID lpParameter) {
    const auto holder = static_cast<Holder*>(lpParameter);
    if (holder->array.size() == 0) {
        return 0;
    }
    int max = INT_MIN, min = INT_MAX;
    int max_index = -1, min_index = -1;
    for (int i = 0; i < holder->array.size(); i++) {
        if (holder->array[i] > max) {
            max = holder->array[i];
            max_index = i;
        }
        Sleep(7);
        if (holder->array[i] < min) {
            min = holder->array[i];
            min_index = i;
        }
        Sleep(7);
    }
    holder->max_index = max_index;
    holder->min_index = min_index;
    return 0;
}
DWORD WINAPI ArrayHandler::findAverage(LPVOID lpParameter) {
    const auto holder = static_cast<Holder*>(lpParameter);
    if (holder->array.size() == 0) {
        return 0;
    }
    double sum = 0;
    for (const int element : holder->array) {
        sum += element;
        Sleep(12);
    }
    holder->average = sum / static_cast<int>(holder->array.size());
    return 0;
}