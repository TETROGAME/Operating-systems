#include "../headers/Request.h"
#include "../headers/solution_namespace.h"
#include <conio.h>
#include <windows.h>

using namespace sln;

int main() {
    cout << "Client: Connecting to server..." << endl;

    HANDLE hPipe = INVALID_HANDLE_VALUE;
    while (hPipe == INVALID_HANDLE_VALUE) {
        WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER);
        hPipe = CreateFile(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, 0, NULL
        );
    }

    cout << "Client: Connected." << endl;

    bool running = true;
    DWORD mode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

    while (running) {
        cout << "\n1. Modify\n2. Read\n3. Exit\nChoice: ";
        int choice;
        cin >> choice;

        Request req;
        Response resp;

        if (choice == 3) {
            req.type = REQ_EXIT;
            SendRequest(hPipe, req);
            running = false;
            continue;
        }

        cout << "Enter Employee ID: ";
        cin >> req.emp_id;

        if (choice == 2) {
            req.type = REQ_READ;
            if (!SendRequest(hPipe, req)) break;
            if (!ReadResponse(hPipe, resp)) break;

            if (resp.success) {
                resp.data.Print();
                cout << "Press any key to finish reading...";
                _getch();
                cout << endl;
            } else {
                cout << "Server: " << resp.message << endl;
            }
        }
        else if (choice == 1) {
            req.type = REQ_MODIFY_START;
            if (!SendRequest(hPipe, req)) break;
            if (!ReadResponse(hPipe, resp)) break;

            if (resp.success) {
                resp.data.Print();
                cout << "Enter new Name: ";
                cin >> req.data.name;
                cout << "Enter new Hours: ";
                cin >> req.data.hours;

                req.data.num = req.emp_id;
                req.type = REQ_MODIFY_COMMIT;

                cout << "Sending update..." << endl;
                SendRequest(hPipe, req);
                if (!ReadResponse(hPipe, resp)) break;

                cout << "Server: " << resp.message << endl;
            } else {
                cout << "Server: " << resp.message << endl;
            }
        }
    }

    CloseHandle(hPipe);
    return 0;
}