# Lab 5 — Named Pipes: Server / Clients (C++98)

Implement coordinated access to a binary employee file by multiple client processes through named pipes. One Server mediates client requests, enforcing per-record read/write access policies.

---

## Objective
Provide concurrent read access and exclusive write access to employee records identified by key (employee ID) through a named-pipe-based protocol.

---

## Requirements (summary)

### Server
- Read from console: the name of the binary data file and initial employee records:
  ```cpp
  struct employee
  {
    int num;        // employee ID
    char name[10];  // employee name
    double hours;   // number of hours worked
  };
  ```
- Create the binary file and write the initial records.
- Print the contents of the created file.
- Read from console the number of Client processes to start and launch them, arranging IPC channels (named pipes).
- Serve client requests ensuring:
  - Modify acquires exclusive access to a record (writers block readers and other writers).
  - Read allows concurrent readers but is blocked by writers requesting/holding exclusive access.
- After all clients finish, print the modified file.
- Allow server termination on a console command.

### Client
- Connect to the Server via named pipe(s).
- Interactive loop:
  - Modify: lock record by key, fetch current value, accept new values, submit changes, release.
  - Read: request record by key, display, release when done.
  - Exit: disconnect and terminate.
- All access by record key (employee ID).

### Additional
- Protocol should include: request type (read/modify/release), record key, and payload (record data or status).
- Include graceful shutdown and error handling (broken pipe, client disconnect, malformed requests).
- Provide a simple scenario with 2–3 clients demonstrating concurrent reads and exclusive writes.

---

## Key Files (suggested)
- `Lab 5/server.cpp`
- `Lab 5/client.cpp`
- `Lab 5/employee.h`, `Lab 5/employee.cpp`
- `Lab 5/FileHandler.h`, `Lab 5/FileHandler.cpp`
- `Lab 5/Request.h`, `Lab 5/Request.cpp`
- `Lab 5/tests`

---

## Build (CMake, Windows)
```bash
cmake -S . -B build
cmake --build build --config Release
```

---

## Run
1) Start the server; create/init the data file and launch clients as needed.
2) Run multiple clients; interact via console to read/modify records.
3) Observe locking behavior and final file contents.

Example:
```bash
./build/server.exe
# In separate terminals
./build/client.exe
./build/client.exe
```

---

## Testing
```bash
ctest --test-dir build -V
```