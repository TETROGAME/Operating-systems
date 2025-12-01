#include "../headers/Employee.h"

Employee::Employee(): num(), name(), hours() {}

Employee::Employee(const int &num, char *name, const double &hours) {
    this->num = num;
    // this->name = name;
    this->hours = hours;
}
