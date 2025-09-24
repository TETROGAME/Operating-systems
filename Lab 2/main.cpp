#include <iostream>
#include <memory>
#include <windows.h>
#include <vector>
using std::cout;
using std::cin;
using std::vector;

struct Holder {
    vector<int> array;
    int min_index{};
    int max_index{};
    int average{};
};

DWORD WINAPI min_max(LPVOID lpParameter) {
    const auto holder = static_cast<Holder*>(lpParameter);
    int max = INT_MIN, min = INT_MAX;
    int max_index = 0, min_index = 0;
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
    cout<<"\nMAX: " << max <<"\nMIN: " << min;
    cout<<"\nMAX index: " << max_index <<"\nMIN index: " << min_index <<"\n";
    holder->max_index = max_index;
    holder->min_index = min_index;
    return 0;
}
DWORD WINAPI average(LPVOID lpParameter) {
    const auto holder = static_cast<Holder*>(lpParameter);
    int sum = 0;
    for (const int element : holder->array) {
        sum += element;
        Sleep(12);
    }
    cout<<"\nAverage: " << sum / holder->array.size()<<"\n";
    holder->average = sum / static_cast<int>(holder->array.size());
    return 0;
}

vector<int> getArray() {
    cout << "Input array size: ";
    int n;
    cin >> n;

    cout << "Enter array elements: ";
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    return arr;
}

int main() {
    auto array = getArray();
    Holder holder = {array, -1, -1, 0};
    HANDLE h_min_max = CreateThread(NULL, 0, min_max, &holder, 0, NULL);
    if (h_min_max == NULL) { return static_cast<int>(GetLastError()); }
    WaitForSingleObject(h_min_max, INFINITE);
    CloseHandle(h_min_max);

    HANDLE h_average = CreateThread(NULL, 0, average, &holder, 0, NULL);
    if (h_average == NULL) { return GetLastError(); }
    WaitForSingleObject(h_average, INFINITE);
    CloseHandle(h_average);

    array[holder.min_index] = holder.average;
    array[holder.max_index] = holder.average;
    cout<<'\n';
    for (const int i : array) {
        cout << i << " ";
    }
    return 0;
}
