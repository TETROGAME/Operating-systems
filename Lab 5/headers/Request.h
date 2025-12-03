#ifndef LAB_5_REQUEST_H
#define LAB_5_REQUEST_H
#include "Employee.h"
#include "solution_namespace.h"
#include <windows.h>

using namespace sln;

static const char* PIPE_NAME = "\\\\.\\pipe\\EmployeePipe";

enum RequestType {
    REQ_READ = 0,
    REQ_MODIFY_START = 1,
    REQ_MODIFY_COMMIT = 2,
    REQ_EXIT = 3
};

struct Request {
    RequestType type;
    int emp_id;
    Employee data;

    Request();
};

struct Response {
    bool success;
    Employee data;
    string message;

    Response();
};

bool SendRequest(HANDLE hPipe, const Request& req);
bool ReadRequest(HANDLE hPipe, Request& req);
bool SendResponse(HANDLE hPipe, const Response& resp);
bool ReadResponse(HANDLE hPipe, Response& resp);

#endif //LAB_5_REQUEST_H