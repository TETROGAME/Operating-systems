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

TEST(MainBehaviorTest, IntendedBehaviourTest) {
    const char* bin_file = "test_employees.bin";
    const char* report_file = "test_report.txt";
    int record_count = 2;
    double pay_per_hour = 20.0;

    remove(bin_file);
    remove(report_file);

    string creator_cmd = string("Creator.exe ") + bin_file + " " + std::to_string(record_count);
    vector<char> cmd1(creator_cmd.begin(), creator_cmd.end());
    cmd1.push_back('\0');
    STARTUPINFOA si_creator = { sizeof(si_creator) };
    PROCESS_INFORMATION pi_creator;
    ZeroMemory(&pi_creator, sizeof(pi_creator));
    ASSERT_TRUE(CreateProcessA(NULL, cmd1.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si_creator, &pi_creator));
    WaitForSingleObject(pi_creator.hProcess, INFINITE);
    CloseHandle(pi_creator.hProcess);
    CloseHandle(pi_creator.hThread);

    ifstream bin(bin_file, std::ios::binary);
    ASSERT_TRUE(bin.is_open());
    employee emp;
    vector<employee> emps;
    while (bin.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        emps.push_back(emp);
    }
    bin.close();
    ASSERT_EQ(emps.size(), record_count);

    std::string reporter_cmd = std::string("Reporter.exe ") + bin_file + " " + report_file + " " + std::to_string(pay_per_hour);
    std::vector<char> cmd2(reporter_cmd.begin(), reporter_cmd.end());
    cmd2.push_back('\0');
    STARTUPINFOA si_reporter = { sizeof(si_reporter) };
    PROCESS_INFORMATION pi_reporter;
    ZeroMemory(&pi_reporter, sizeof(pi_reporter));
    ASSERT_TRUE(CreateProcessA(NULL, cmd2.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si_reporter, &pi_reporter));
    WaitForSingleObject(pi_reporter.hProcess, INFINITE);
    CloseHandle(pi_reporter.hProcess);
    CloseHandle(pi_reporter.hThread);

    ifstream rep(report_file);
    ASSERT_TRUE(rep.is_open());
    std::string line, content;
    while (std::getline(rep, line)) {
        content += line + "\n";
    }
    rep.close();
    for (const auto& e : emps) {
        EXPECT_NE(content.find(e.name), std::string::npos);
        double expected_salary = e.hours * pay_per_hour;
        char salary_str[32];
        snprintf(salary_str, sizeof(salary_str), "%.2f", expected_salary);
        EXPECT_NE(content.find(salary_str), std::string::npos);
    }

    remove(bin_file);
    remove(report_file);
}