#include <iostream>
#include <memory>
#include <windows.h>
#include <vector>
#include "ArrayHandler.h"
using std::cout;
using std::cin;
using std::vector;



int main() {
    Holder holder (ArrayHandler::getArray(), -1, -1, 0 );
    HANDLE h_min_max = CreateThread(NULL, 0, ArrayHandler::findMinMax, &holder, 0, NULL);
    if (h_min_max == NULL) { return static_cast<int>(GetLastError()); }
    WaitForSingleObject(h_min_max, INFINITE);
    CloseHandle(h_min_max);

    cout<<"\nMAX: " << holder.array[holder.min_index] <<"\nMIN: " << holder.array[holder.max_index];
    cout<<"\nMAX index: " << holder.max_index <<"\nMIN index: " << holder.min_index <<"\n";

    HANDLE h_average = CreateThread(NULL, 0, ArrayHandler::findAverage, &holder, 0, NULL);
    if (h_average == NULL) { return static_cast<int>(GetLastError()); }
    WaitForSingleObject(h_average, INFINITE);
    CloseHandle(h_average);
    cout<<"\nAverage: " << holder.average <<"\n";

    holder.array[holder.min_index] = holder.average;
    holder.array[holder.max_index] = holder.average;
    cout<<'\n';
    for (const int i : holder.array) {
        cout << i << " ";
    }
    return 0;
}
