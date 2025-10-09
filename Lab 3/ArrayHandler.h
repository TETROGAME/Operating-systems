#ifndef ARRAYHANDLER_H
#define ARRAYHANDLER_H
#include <iostream>
#include <memory>
using std::cin;
using std::cout;
using std::unique_ptr;

class ArrayHandler {
public:
     ArrayHandler() = delete;

     static unique_ptr<int[]> initializeArray();
};



#endif //ARRAYHANDLER_H
