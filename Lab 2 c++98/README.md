# Lab 2 — Array Processing with Multithreading (C++98)

Demonstrate parallel processing and synchronization techniques using a C++98-compatible approach.

---

## Objective
Process integer arrays concurrently to compute minimum, maximum, and average values, then update the array by replacing min/max elements with the average.

---

## Tasks
- Implement an array handler to read and process integer arrays.
- Use threads to find minimum, maximum, and average values.
- Replace the minimum and maximum elements with the average value.
- Provide end-to-end and unit tests for all operations, including edge cases.

---

## Key Files
- `Lab 2 c++98/main.cpp`
- `Lab 2 c++98/ArrayHandler.cpp`, `Lab 2 c++98/ArrayHandler.h`
- `Lab 2 c++98/ThreadHandler.cpp`, `Lab 2 c++98/ThreadHandler.h`
- `Lab 2 c++98/test.cpp`

---

## Build (CMake, Windows)
```bash
cmake -S . -B build
cmake --build build --config Release
```

Toolchain/standard:
- C++98
- Windows toolchain; use threading primitives appropriate for your environment.

---

## Run
Typical flow:
1) Load or generate input array.
2) Launch threads to compute min, max, and average.
3) Update the array in place.
4) Output results.

Example:
```bash
./build/lab2_c98.exe
```

---

## Testing
```bash
ctest --test-dir build -V
```
---

## Notes
- Ensure proper synchronization when sharing data among threads.
- Guard against empty arrays and overflow in average computation.