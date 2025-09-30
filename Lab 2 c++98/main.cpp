#include <iostream>
#include <vector>
#include "ArrayHandler.h"
#include "ThreadHandler.h"
using std::cout;
using std::cin;
using std::vector;

int main() {
    Holder holder (ArrayHandler::getArray(), -1, -1, 0 );

    ThreadHandler::findMinMaxThread(holder);
    cout<<"\nMAX: " << holder.array[holder.min_index] <<"\nMIN: " << holder.array[holder.max_index];
    cout<<"\nMAX index: " << holder.max_index <<"\nMIN index: " << holder.min_index <<"\n";

    ThreadHandler::findAverageThread(holder);
    cout<<"\nAverage: " << holder.average <<"\n";

    ArrayHandler::setMinMaxToAverage(holder);
    for (int i = 0; i < holder.array.size(); i++) { cout << holder.array[i] << " "; }
    return 0;
}
