#ifndef ARRAYHANDLER_H
#define ARRAYHANDLER_H
#include <vector>
#include "Holder.h"
using std::vector;

namespace ArrayHandler {
    vector<int> getArray();
    void setMinMaxToAverage(Holder& holder);
    void findMinMax(Holder& holder);
    void findAverage(Holder& holder);
};

#endif //ARRAYHANDLER_H
