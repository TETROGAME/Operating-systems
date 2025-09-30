#ifndef ARRAYHANDLER_H
#define ARRAYHANDLER_H

#include <vector>
#include <windows.h>
#include "Holder.h"
using std::vector;
namespace ArrayHandler {
    vector<int> getArray();
    void setMinMaxToAverage(Holder& holder);
    DWORD WINAPI findMinMax(LPVOID lpParameter);
    DWORD WINAPI findAverage(LPVOID lpParameter);
};

#endif //ARRAYHANDLER_H
