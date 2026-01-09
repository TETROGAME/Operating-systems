# Lab 2 — Array Processing with Multithreading (C++20)

Demonstrate parallel processing and synchronization techniques using modern C++ (C++20).

---

## Objective
Process integer arrays concurrently to compute minimum, maximum, and average values, then update the array by replacing min/max elements with the average. Contrast with the C++98 variant to highlight language evolution.

---

## Tasks
- Implement an array handler to read and process integer arrays.
- Use threads to find minimum, maximum, and average values.
- Replace the minimum and maximum elements with the average value.
- Provide end-to-end and unit tests for all operations, including edge cases.

---

## Key Files
- `Lab 2 c++20/main.cpp`
- `Lab 2 c++20/ArrayHandler.cpp`, `Lab 2 c++20/ArrayHandler.h`
- `Lab 2 c++20/ThreadHandler.cpp`, `Lab 2 c++20/ThreadHandler.h`
- `Lab 2 c++20/test.cpp`

---

## Build (CMake, Windows)
```bash
cmake -S . -B build -DCMAKE_CXX_STANDARD=20
cmake --build build --config Release
```

Toolchain/standard:
- C++20

---

## Run
Example:
```bash
./build/lab2_c20.exe
```

---

## Testing
```bash
ctest --test-dir build -V
```