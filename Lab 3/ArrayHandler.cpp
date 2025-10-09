#include "ArrayHandler.h"

unique_ptr<int[]> ArrayHandler::getArray() {
    int size = 0;
    cout << "Input array size: ";
    cin >> size;
    unique_ptr<int[]> array(new int[size]);
    cout << "Input " << size << " array Elements: ";
    for (int i = 0; i < size; i++) {
        cin >> array[i];
    }
    return array;
}
