#pragma once
#include <vector>
#pragma once
struct employee {
    int num;
    char name[10];
    double hours;
};

vector<employee> ReadEmployeesFromBinary(const string& filename);
bool WriteEmployeesToBinary(const string& filename, const vector<employee>& employees);
vector<string> ReadReportFile(const string& filename);