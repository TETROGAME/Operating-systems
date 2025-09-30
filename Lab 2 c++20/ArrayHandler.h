#ifndef ARRAYHANDLER_H
#define ARRAYHANDLER_H
#include <vector>
#include "Holder.h"
using std::vector;

class ArrayHandler {
public:
    static vector<int> getArray();
    static void setMinMaxToAverage(Holder& holder);
    static void findMinMax(Holder& holder);
    static void findAverage(Holder& holder);
    ArrayHandler() = delete;
};

#endif //ARRAYHANDLER_H
