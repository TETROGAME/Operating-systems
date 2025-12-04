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

}