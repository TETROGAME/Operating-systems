#include "employee_io.h"
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

vector<employee> ReadEmployeesFromBinary(const string& filename) {
    vector<employee> result;
    ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) return result;
    employee emp;
    while (fin.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        result.push_back(emp);
    }
    return result;
}

bool WriteEmployeesToBinary(const string& filename, const vector<employee>& employees) {
    ofstream fout(filename, std::ios::binary);
    if (!fout.is_open()) return false;
    for (const auto& emp : employees) {
        fout.write(reinterpret_cast<const char*>(&emp), sizeof(emp));
    }
    return true;
}

vector<string> ReadReportFile(const string& filename) {
    vector<string> lines;
    ifstream fin(filename);
    string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}
