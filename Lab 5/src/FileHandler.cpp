#include "../headers/FileHandler.h"

using namespace sln;

FileHandler::FileHandler(const string& filename) : _filename(filename) {}

void FileHandler::CreateFileWithData(const vector<Employee>& data) {
    ofstream ofs(_filename.c_str(), ios::binary | ios::out | ios::trunc);
    if (!ofs.is_open()) return;

    for (size_t i = 0; i < data.size(); ++i) {
        const Employee& e = data[i];
        ofs.write(reinterpret_cast<const char*>(&e.num), sizeof(int));
        unsigned int name_len = static_cast<unsigned int>(e.name.size());
        ofs.write(reinterpret_cast<const char*>(&name_len), sizeof(unsigned int));
        if (name_len > 0) ofs.write(e.name.c_str(), name_len);
        ofs.write(reinterpret_cast<const char*>(&e.hours), sizeof(double));
    }
    ofs.close();
}

void FileHandler::ReadAllAndPrint() {
    ifstream ifs(_filename.c_str(), ios::binary | ios::in);
    if (!ifs.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }

    cout << "\n--- File Contents ---" << endl;
    while (true) {
        int num;
        ifs.read(reinterpret_cast<char*>(&num), sizeof(int));
        if (!ifs) break;
        unsigned int name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(unsigned int));
        string name;
        if (name_len > 0) {
            name.resize(name_len);
            ifs.read(&name[0], name_len);
        }
        double hours;
        ifs.read(reinterpret_cast<char*>(&hours), sizeof(double));
        if (!ifs) break;
        Employee e(num, name, hours);
        e.Print();
    }
    cout << "---------------------\n" << endl;
    ifs.close();
}

bool FileHandler::GetEmployee(int id, Employee& outEmp) {
    ifstream ifs(_filename.c_str(), ios::binary | ios::in);
    if (!ifs.is_open()) return false;

    while (true) {
        int num;
        ifs.read(reinterpret_cast<char*>(&num), sizeof(int));
        if (!ifs) break;
        unsigned int name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(unsigned int));
        string name;
        if (name_len > 0) {
            name.resize(name_len);
            ifs.read(&name[0], name_len);
        }
        double hours;
        ifs.read(reinterpret_cast<char*>(&hours), sizeof(double));
        if (!ifs) break;
        if (num == id) {
            outEmp.num = num;
            outEmp.name = name;
            outEmp.hours = hours;
            ifs.close();
            return true;
        }
    }
    ifs.close();
    return false;
}

bool FileHandler::UpdateEmployee(const Employee& emp) {
    ifstream ifs(_filename.c_str(), ios::binary | ios::in);
    if (!ifs.is_open()) return false;

    vector<Employee> all;
    while (true) {
        int num;
        ifs.read(reinterpret_cast<char*>(&num), sizeof(int));
        if (!ifs) break;
        unsigned int name_len;
        ifs.read(reinterpret_cast<char*>(&name_len), sizeof(unsigned int));
        string name;
        if (name_len > 0) {
            name.resize(name_len);
            ifs.read(&name[0], name_len);
        }
        double hours;
        ifs.read(reinterpret_cast<char*>(&hours), sizeof(double));
        if (!ifs) break;
        all.push_back(Employee(num, name, hours));
    }
    ifs.close();

    bool found = false;
    for (size_t i = 0; i < all.size(); ++i) {
        if (all[i].num == emp.num) {
            all[i] = emp;
            found = true;
            break;
        }
    }
    if (!found) return false;

    CreateFileWithData(all);
    return true;
}