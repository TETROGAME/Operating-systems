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
class ArrayHandler {
public:
    static vector<int> getArray();
    static void setMinMaxToAverage(Holder& holder);
    static DWORD WINAPI findMinMax(LPVOID lpParameter);
    static DWORD WINAPI findAverage(LPVOID lpParameter);
};

#endif //ARRAYHANDLER_H
