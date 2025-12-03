#include "../headers/Employee.h"

Employee::Employee() : num(0), name(), hours(0.0) {}

Employee::Employee(int n, const string& name_str, double h)
    : num(n), name(name_str), hours(h) {}

void Employee::Print() const {
    cout << "ID: " << num << "\tName: " << name << "\tHours: " << hours << endl;
}