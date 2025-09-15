#include "pch.h"
#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <gtest/gtest.h>
#include "../Reporter/employee.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

int generate_report(const string& bin_file, const string& rep_file, double pay) {
    ifstream fin(bin_file, std::ios::binary);
    if (!fin.is_open()) {
        return 2;
    }
    ofstream fout(rep_file);
    if (!fout) {
        return 3;
    }

    fout << "Report for \"" << bin_file << "\" file\n";
    fout << "----------------------------------------------------\n";
    fout << "Num\tName\tHours\tSalary\n";

    employee emp;
    while (fin.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        double salary = emp.hours * pay;
        fout << emp.num << "\t" << emp.name << "\t"
            << emp.hours << "\t" << std::fixed << std::setprecision(2) << salary << "\n";
    }
    fin.close();
    fout.close();
    return 0;
}

TEST(ReporterTest, GeneratesReportFile) {
    const char* test_bin = "test_employee.bin";
    const char* test_report = "test_report.txt";
    employee emp = {1, "John", 40.0};
    {
        ofstream fout(test_bin, std::ios::binary);
        fout.write(reinterpret_cast<const char*>(&emp), sizeof(emp));
    }

    int result = generate_report(test_bin, test_report, 10.0);

    EXPECT_EQ(result, 0);

    ifstream fin(test_report);
    string content, line;
    while (std::getline(fin, line)) {
        content += line + "\n";
    }
    EXPECT_NE(content.find("John"), string::npos);
    EXPECT_NE(content.find("400.00"), string::npos);

    remove(test_bin);
    remove(test_report);
}

TEST(ReporterTest, ReturnsErrorWhenBinaryFileMissing) {
    const char* missing_bin = "nonexistent.bin";
    const char* test_report = "should_not_create.txt";

    int result = generate_report(missing_bin, test_report, 10.0);

    EXPECT_EQ(result, 2);

    std::ifstream fin(test_report);
    EXPECT_FALSE(fin.is_open());

    remove(test_report);
}
