#include "ThreadHandler.h"
#include <iostream>

void ThreadHandler::findMinMaxThread(Holder &holder) {
    HANDLE h_min_max = CreateThread(NULL, 0, ArrayHandler::findMinMax, &holder, 0, NULL);
    if (h_min_max == NULL) { std::cout << "findMinMaxThread Error: " << static_cast<int>(GetLastError()); }
    WaitForSingleObject(h_min_max, INFINITE);
    CloseHandle(h_min_max);
}
void ThreadHandler::findAverageThread(Holder &holder) {
    HANDLE h_average = CreateThread(NULL, 0, ArrayHandler::findAverage, &holder, 0, NULL);
    if (h_average == NULL) { std::cout << "findAverageThread Error: " << static_cast<int>(GetLastError()); }
    WaitForSingleObject(h_average, INFINITE);
    CloseHandle(h_average);
}
