#include <iostream>
#include <vector>
#include "ArrayHandler.h"
#include "ThreadHandler.h"
using std::cout;
using std::cin;
using std::vector;
using namespace ArrayHandler;
using namespace ThreadHandler;
int main() {
    Holder holder (getArray(), -1, -1, 0 );

    launchFindMinMaxThread(
        findMinMax, holder);
    cout<<"\nMAX: " << holder.array[holder.min_index] <<"\nMIN: " << holder.array[holder.max_index];
    cout<<"\nMAX index: " << holder.max_index <<"\nMIN index: " << holder.min_index <<"\n";

    launchFindAverageThread(
        findAverage, holder);
    cout<<"\nAverage: " << holder.average <<"\n";

    setMinMaxToAverage(holder);
    for (const int i : holder.array) { cout << i << " "; }
    return 0;
}
