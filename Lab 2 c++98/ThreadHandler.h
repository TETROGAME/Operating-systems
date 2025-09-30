#ifndef THREADHANDLER_H
#define THREADHANDLER_H

#include "ArrayHandler.h"

class ThreadHandler {
public:
    static void findMinMaxThread(Holder& holder);
    static void findAverageThread(Holder& holder);
    ThreadHandler() = delete;
};

#endif //THREADHANDLER_H
