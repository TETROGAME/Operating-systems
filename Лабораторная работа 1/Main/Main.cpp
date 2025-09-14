#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "employee.h"

void PrintBinaryFile(std::string filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        std::cerr << "Can't open \"" << filename << "\" file\n";
        return;
    }
    std::cout << "File content:\n";
    employee emp;
    while (fin.read(reinterpret_cast<char*>(&emp), sizeof(emp))) {
        std::cout << emp.num << " " << emp.name << " " << emp.hours << std::endl;
    }
    fin.close();
}

void PrintReport(std::string filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Can't open report file.\n";
        return;
    }
    std::string line;
    while (std::getline(fin, line)) {
        std::cout << line << std::endl;
    }
    fin.close();
}

int main() {
    std::string creator_file, report_file;
    int record_count;
    double pay_per_hour;

    std::cout << "Enter binary file name: ";
    std::cin >> creator_file;
    std::cout << "Enter number of records: ";
    std::cin >> record_count;

    //Creator.exe
    std::string creatorCommand = std::string("Creator.exe") + ' ' + creator_file + ' ' + std::to_string(record_count);
    std::vector<char> cmd1(creatorCommand.begin(), creatorCommand.end());
    cmd1.push_back('\0');

    STARTUPINFOA si_creator;
    PROCESS_INFORMATION pi_creator;
    ZeroMemory(&si_creator, sizeof(si_creator));
    si_creator.cb = sizeof(si_creator);
    ZeroMemory(&pi_creator, sizeof(pi_creator));

    if (!CreateProcessA(NULL, cmd1.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si_creator, &pi_creator)) {
        std::cerr << "Failed to start Creator.exe. Error: " << GetLastError() << "\n";
        return 1;
    }
    WaitForSingleObject(pi_creator.hProcess, INFINITE);
    CloseHandle(pi_creator.hProcess);
    CloseHandle(pi_creator.hThread);

    PrintBinaryFile(creator_file);


    std::cout << "Enter report file name: ";
    std::cin >> report_file;
    std::cout << "Enter pay per hour: ";
    std::cin >> pay_per_hour;

    //Reporter.exe
    std::string reporterCommand = std::string("Reporter.exe") + ' ' + creator_file + ' ' + report_file + ' ' + std::to_string(pay_per_hour);
    std::vector<char> cmd2(reporterCommand.begin(), reporterCommand.end());
    cmd1.push_back('\0');
    STARTUPINFOA si_reporter;
    PROCESS_INFORMATION pi_reporter;
    ZeroMemory(&si_reporter, sizeof(si_reporter));
    si_reporter.cb = sizeof(si_reporter);
    ZeroMemory(&pi_reporter, sizeof(pi_reporter));

    if (!CreateProcessA(NULL, cmd2.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si_reporter, &pi_reporter)) {
        std::cerr << "Failed to start Reporter.exe. Error: " << GetLastError() << "\n";
        return 1;
    }
    WaitForSingleObject(pi_reporter.hProcess, INFINITE);
    CloseHandle(pi_reporter.hProcess);
    CloseHandle(pi_reporter.hThread);

    PrintReport(report_file);

    return 0;
}