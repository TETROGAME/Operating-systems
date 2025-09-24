#include <iostream>
#include <memory>
#include <windows.h>
#include <vector>
using std::cout;
using std::cin;
using std::vector;

DWORD WINAPI min_max(LPVOID lpParameter) {
    const auto array_ptr = static_cast<vector<int>*>(lpParameter);
    auto array = *array_ptr;
    int max = INT_MIN, min = INT_MAX;
    int max_index = 0, min_index = 0;
    for (int i = 0; i < array.size(); i++) {
        if (array[i] > max) {
            max = array[i];
            max_index = i;
        }
        Sleep(7);
        if (array[i] < min) {
            min = array[i];
            min_index = i;
        }
        Sleep(7);
    }
    cout<<"\nMAX: " << max <<"\nMIN: " << min;
    cout<<"\nMAX index: " << max_index <<"\nMIN index: " << min_index <<"\n";
    DWORD result = (min_index << 16) | max_index;
    return result;
}
DWORD WINAPI average(LPVOID lpParameter) {
    const auto array_ptr = static_cast<vector<int>*>(lpParameter);
    auto array = *array_ptr;
    int sum = 0;
    for (int element : array) {
        sum += element;
        Sleep(12);
    }
    cout<<"\nAverage: " << sum/array.size()<<"\n";
    return sum/array.size();
}

vector<int> getArray() {
    cout << "Input array size: ";
    int n;
    cin >> n;

    cout << "Enter array elements: ";
    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    return arr;
}

int main() {
    DWORD exit_code;
    auto array = getArray();

    HANDLE h_min_max = CreateThread(NULL, 0, min_max, &array, 0, NULL);
    if (h_min_max == NULL) { return GetLastError(); }
    WaitForSingleObject(h_min_max, INFINITE);
    GetExitCodeThread(h_min_max, &exit_code);
    CloseHandle(h_min_max);

    DWORD answer = exit_code;
    int max_index = static_cast<int>(answer & 0xFFFF);
    int min_index = static_cast<int>((answer >> 16) & 0xFFFF);
    cout << "\nMinimum: " << array[min_index] << "\nMaximum: " << array[max_index];

    HANDLE h_average = CreateThread(NULL, 0, average, &array, 0, NULL);
    if (h_average == NULL) { return GetLastError(); }
    WaitForSingleObject(h_average, INFINITE);
    GetExitCodeThread(h_average, &exit_code);
    int avg = exit_code;
    CloseHandle(h_average);

    array[min_index] = avg;
    array[max_index] = avg;
    cout<<'\n';
    for (const int i : array) {
        cout << i << " ";
    }
    return 0;
}
