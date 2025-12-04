#include "../headers/Request.h"
#include "../headers/FileHandler.h"
#include "../headers/solution_namespace.h"
#include <windows.h>

using namespace sln;

void ProcessClient(HANDLE hPipe, FileHandler& fileHelper) {
    while (true) {
        Request req;
        if (!ReadRequest(hPipe, req)) break;

        Response resp;
        resp.success = false;
        resp.data = Employee();
        resp.message = "";

        if (req.type == REQ_READ) {
            cout << "Server: Read request for ID " << req.emp_id << endl;
            Employee e;
            if (fileHelper.GetEmployee(req.emp_id, e)) {
                resp.success = true;
                resp.data = e;
                resp.message = "Read success";
            } else {
                resp.message = "ID not found";
            }
        } else if (req.type == REQ_MODIFY_START) {
            cout << "Server: Modify Start for ID " << req.emp_id << endl;
            Employee e;
            if (fileHelper.GetEmployee(req.emp_id, e)) {
                resp.success = true;
                resp.data = e;
                resp.message = "Record ready for modification";
            } else {
                resp.message = "ID not found";
            }
        } else if (req.type == REQ_MODIFY_COMMIT) {
            cout << "Server: Modify Commit for ID " << req.data.num << endl;
            if (fileHelper.UpdateEmployee(req.data)) {
                resp.success = true;
                resp.message = "Update success";
            } else {
                resp.message = "Update failed";
            }
        } else if (req.type == REQ_EXIT) {
            cout << "Server: Client exit" << endl;
            resp.success = true;
            resp.message = "Goodbye";
            SendResponse(hPipe, resp);
            break;
        }

        if (!SendResponse(hPipe, resp)) {
            break;
        }
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
}

int main() {
    string filename;
    int count;

    cout << "Enter filename: ";
    cin >> filename;

    cout << "Enter number of employees: ";
    cin >> count;

    vector<Employee> employees;
    for(int i = 0; i < count; ++i) {
        int id; string name; double h;
        cout << "Employee " << (i+1) << " ID: "; cin >> id;
        cout << "Employee " << (i+1) << " Name: "; cin >> name;
        cout << "Employee " << (i+1) << " Hours: "; cin >> h;
        employees.push_back(Employee(id, name, h));
    }

    FileHandler fh(filename);
    fh.CreateFileWithData(employees);
    fh.ReadAllAndPrint();

    cout << "Enter number of client processes to start: ";
    int clients;
    cin >> clients;

    for(int i = 0; i < clients; ++i) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        char cmdLine[] = "client.exe";
        if (!CreateProcess(NULL, cmdLine, NULL, NULL, FALSE,
                           CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            cout << "Failed to create client process." << endl;
        } else {
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    cout << "Server listening on pipe..." << endl;

    for(int i=0; i < clients; ++i) {
        HANDLE hPipe = CreateNamedPipe(
            PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            4096, 4096, 0, NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            cout << "Failed to create pipe." << endl;
            return 1;
        }

        if (ConnectNamedPipe(hPipe, NULL) != FALSE) {
            cout << "Client connected." << endl;
            ProcessClient(hPipe, fh);
        } else {
             CloseHandle(hPipe);
        }
    }

    cout << "All clients finished." << endl;
    fh.ReadAllAndPrint();

    cout << "Press any key to exit server...";
    char c; cin >> c;
    return 0;
}