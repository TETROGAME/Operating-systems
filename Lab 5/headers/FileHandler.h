#ifndef LAB_5_FILEHELPER_H
#define LAB_5_FILEHELPER_H


#include "Employee.h"
#include "solution_namespace.h"

using namespace sln;

class FileHandler {
public:
    FileHandler(const string& filename);
    void CreateFileWithData(const vector<Employee>& data);
    void ReadAllAndPrint();
    bool GetEmployee(int id, Employee& outEmp);
    bool UpdateEmployee(const Employee& emp);

private:
    string _filename;
};


#endif //LAB_5_FILEHELPER_H