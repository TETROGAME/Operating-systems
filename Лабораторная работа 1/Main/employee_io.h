#pragma once
#include <string>
#include <vector>
#include "employee.h"
using std::string, std::vector;

vector<employee> ReadEmployeesFromBinary(const string& filename);
bool WriteEmployeesToBinary(const string& filename, const vector<employee>& employees);
vector<string> ReadReportFile(const string& filename);