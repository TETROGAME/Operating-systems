# Operating Systems

---

## Project Overview

- **Build System:** [CMake](https://cmake.org/)
- **Testing Framework:** [GoogleTest](https://github.com/google/googletest)
- **Languages:** C++, CMake
- **Platform:** Windows

---

## Task Progress

> **Lab 1:** Complete  
> **Lab 2 (C++98):** Complete  
> **Lab 2 (C++20):** Complete  
> **Lab 3:** Yet to be released  
> **Lab 4:** Yet to be released  
> **Lab 5:** Yet to be released

---

## Lab Task Definitions

### Lab 1: Employee Database & Reporting

**Objective:**  
Design a system to manage employee records and generate payroll reports based on binary data files.

**Tasks:**
- Implement `Creator.exe` to create a binary file of employee records.
- Implement `Reporter.exe` to read the binary file and generate a formatted report, calculating the salary for each employee.
- Main program orchestrates both creator and reporter modules.
- Error handling for missing files, invalid paths, and report generation.
- Unit tests for core functionalities: file creation, reading, error cases.

**Key Files:**
- `Lab 1/Main.cpp`
- `Lab 1/Creator.cpp`
- `Lab 1/Reporter.cpp`
- `Lab 1/employee.h/.cpp`
- `Lab 1/test.cpp`

---

### Lab 2: Array Processing with Multithreading (C++98 & C++20)

**Objective:**  
Demonstrate parallel processing and synchronization techniques in classic and modern C++.

**Tasks:**
- Implement an array handler to read and process integer arrays.
- Find minimum, maximum, and average values using threads.
- C++98 and C++20 implementations to highlight language evolution.
- Set minimum and maximum elements to average value in the array.
- End-to-end and unit tests for all operations, including edge cases.

**Key Files:**

C++98 version:
- `Lab 2 c++98/main.cpp`
- `Lab 2 c++98/ArrayHandler.cpp/.h`
- `Lab 2 c++98/ThreadHandler.cpp/.h`
- `Lab 2 c++98/test.cpp`
- 
C++20 version:
- `Lab 2 c++20/main.cpp`
- `Lab 2 c++20/ArrayHandler.cpp/.h`
- `Lab 2 c++20/ThreadHandler.cpp/.h`
- `Lab 2 c++20/test.cpp`


---

## Directory Structure

```
Operating-systems/
├── Lab 1/
│   ├── Main.cpp
│   ├── Creator.cpp
│   ├── Reporter.cpp
│   ├── employee.h
│   ├── employee.cpp
│   └── test.cpp
├── Lab 2 c++98/
│   ├── main.cpp
│   ├── ArrayHandler.cpp
│   ├── ArrayHandler.h
│   ├── ThreadHandler.cpp
│   ├── ThreadHandler.h
│   └── test.cpp
├── Lab 2 c++20/
│   ├── main.cpp
│   ├── ArrayHandler.cpp
│   ├── ArrayHandler.h
│   ├── ThreadHandler.cpp
│   ├── ThreadHandler.h
│   └── test.cpp
├── Lab 3/
│   └── ...
├── Lab 4/
│   └── ...
├── Lab 5/
│   └── ...
└── CMakeLists.txt
```


