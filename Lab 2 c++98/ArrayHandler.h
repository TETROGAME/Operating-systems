#ifndef ARRAYHANDLER_H
#define ARRAYHANDLER_H

#include <vector>
#include <windows.h>
using std::vector;

struct Holder {
    vector<int> array;
    int min_index;
    int max_index;
    double average;
    Holder();
    Holder(const vector<int>& array, int min_index, int max_index, double average);
};
namespace ArrayHandler {
    vector<int> getArray();
    void setMinMaxToAverage(Holder& holder);
    DWORD WINAPI findMinMax(LPVOID lpParameter);
    DWORD WINAPI findAverage(LPVOID lpParameter);
};

#endif //ARRAYHANDLER_H
