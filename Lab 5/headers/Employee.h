//
// Created by User on 01.12.2025.
//

#ifndef LAB_5_EMPLOYEE_H
#define LAB_5_EMPLOYEE_H


class Employee {
private:
    int num;
    char name[10];
    double hours;
public:
    Employee();

    Employee(const int& num, char* name, const double& hours);
};


#endif //LAB_5_EMPLOYEE_H