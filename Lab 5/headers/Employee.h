//
// Created by User on 01.12.2025.
//

#ifndef LAB_5_EMPLOYEE_H
#define LAB_5_EMPLOYEE_H

#include "solution_namespace.h"
using namespace sln;

struct Employee {
    int num;
    string name;
    double hours;

    Employee();
    Employee(int n, const string& name_str, double h);
    void Print() const;
};


#endif //LAB_5_EMPLOYEE_H