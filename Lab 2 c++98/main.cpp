#include <iostream>
#include <vector>
#include "ArrayHandler.h"
#include "ThreadHandler.h"
using std::cout;
using std::cin;
using std::vector;
using namespace ThreadHandler;
using namespace ArrayHandler;
int main() {
    Holder holder (getArray(), -1, -1, 0 );

    findMinMaxThread(holder);
    cout<<"\nMAX: " << holder.array[holder.min_index] <<"\nMIN: " << holder.array[holder.max_index];
    cout<<"\nMAX index: " << holder.max_index <<"\nMIN index: " << holder.min_index <<"\n";

    findAverageThread(holder);
    cout<<"\nAverage: " << holder.average <<"\n";

    setMinMaxToAverage(holder);
    for (int i = 0; i < holder.array.size(); i++) { cout << holder.array[i] << ' '; }
    return 0;
}
