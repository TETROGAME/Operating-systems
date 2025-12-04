#include "gtest/gtest.h"
#include "../headers/Request.h"
#include "../headers/FileHandler.h"
#include "../headers/solution_namespace.h"
#include <windows.h>

using namespace sln;

DWORD WINAPI MockServerThread(LPVOID lpParam) {
    FileHandler* fh = static_cast<FileHandler*>(lpParam);
    
    HANDLE hPipe = CreateNamedPipe(
        PIPE_NAME,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        8192, 8192, 0, NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) return 1;

    if (ConnectNamedPipe(hPipe, NULL)) {
        Request req;
        Response resp;
        
        if (!ReadRequest(hPipe, req)) {
            FlushFileBuffers(hPipe);
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
            return 1;
        }
        
        if (req.type == REQ_READ) {
            Employee e;
            if (fh->GetEmployee(req.emp_id, e)) {
                resp.success = true;
                resp.data = e;
                resp.message = "OK";
            } else {
                resp.success = false;
                resp.message = "Not found";
            }
            SendResponse(hPipe, resp);
        }
    }
    
    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return 0;
}

class E2ETest : public ::testing::Test {
protected:
    string test_file;

    void SetUp() {
        test_file = string("test_e2e_gtest.bin");
    }

    void TearDown() {
        remove(test_file.c_str());
    }
};

TEST_F(E2ETest, ClientServerInteraction) {
    vector<Employee> data;
    data.push_back(Employee(100, string("ServerUser"), 50.0));
    FileHandler fh(test_file);
    fh.CreateFileWithData(data);

    HANDLE hThread = CreateThread(NULL, 0, MockServerThread, &fh, 0, NULL);
    ASSERT_NE(hThread, static_cast<HANDLE>(NULL));
    
    Sleep(100);

    HANDLE hPipe = CreateFile(
        PIPE_NAME,
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL
    );

    ASSERT_NE(hPipe, INVALID_HANDLE_VALUE) << "Failed to connect to pipe";

    DWORD mode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);

    Request req;
    req.type = REQ_READ;
    req.emp_id = 100;
    
    ASSERT_TRUE(SendRequest(hPipe, req));

    Response resp;
    ASSERT_TRUE(ReadResponse(hPipe, resp));

    EXPECT_TRUE(resp.success);
    EXPECT_EQ(resp.data.num, 100);
    EXPECT_EQ(resp.data.name, string("ServerUser"));

    CloseHandle(hPipe);
    WaitForSingleObject(hThread, 2000);
    CloseHandle(hThread);
}