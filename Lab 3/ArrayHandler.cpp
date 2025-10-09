#include "ArrayHandler.h"

unique_ptr<int[]> ArrayHandler::initializeArray() {
    int size = 0;
    cout << "Input array size: ";
    cin >> size;
    unique_ptr<int[]> array(new int[size]);
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
    return array;
}
