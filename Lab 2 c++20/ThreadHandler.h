#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include "ArrayHandler.h"
#include <functional>

class ThreadHandler {
public:
    static void launchFindMinMaxThread(
        const std::function<void(Holder&)>& findMinMax, Holder &holder);
    static void launchFindAverageThread(
        const std::function<void(Holder&)>& findAverage, Holder &holder);
};

#endif //THREADHANDLER_H
