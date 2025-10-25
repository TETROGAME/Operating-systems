#include <iostream>
#include "include/Array.h"
#include "include/ThreadHandler.h"

using std::cout;
using std::cin;

int main() {
    int size = 0;
    cout << "Input array size: ";
    cin >> size;
    Array array;
    try {
        array = Array(size);
    } catch (const std::invalid_argument& exception) {
        cout << "Error: " << exception.what() << '\n';
        return 1;
    }

    for (int& elem : array) {
        elem = 0;
    }
    cout << "Initial array: ";
    for (const int& elem : array) {
        cout << elem << ' ';
    }
    cout << "\n";

    int thread_count = 0;
    cout << "Input desired thread number: ";
    cin >> thread_count;
    if (thread_count <= 0) {
        cout << "Error: Thread count must be a positive integer.\n";
        return 1;
    }

    ThreadHandler handler(thread_count, array);
    handler.run();

    return 0;
}