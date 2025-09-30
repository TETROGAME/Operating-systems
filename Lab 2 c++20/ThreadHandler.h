#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include "ArrayHandler.h"
#include <functional>

namespace ThreadHandler {
    void launchFindMinMaxThread(
        const std::function<void(Holder&)>& findMinMax, Holder &holder);
    void launchFindAverageThread(
        const std::function<void(Holder&)>& findAverage, Holder &holder);
};

#endif //THREADHANDLER_H
