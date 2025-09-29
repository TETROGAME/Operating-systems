#include "ThreadHandler.h"
#include <thread>
using std::thread;

void ThreadHandler::launchFindMinMaxThread(const std::function<void(Holder&)>& findMinMax, Holder& holder) {
    thread findMinMaxThread(std::ref(findMinMax), std::ref(holder));
    findMinMaxThread.join();
}
void ThreadHandler::launchFindAverageThread(const std::function<void(Holder&)>& findAverage, Holder &holder) {
    thread findAverageThread(std::ref(findAverage), std::ref(holder));
    findAverageThread.join();
}
