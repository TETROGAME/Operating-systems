# Lab 3 — Thread Synchronization (simplified) [C++20]

Create a console program where the main thread coordinates several marker threads working over a shared integer array.

---

## Objective
Demonstrate synchronization using mutexes/critical sections and condition variables to coordinate work start and completion across multiple threads.

---

## Tasks
1. Read the array size from the console; allocate an integer array of that size and initialize all elements to zero.
2. Ask the user for the number of marker threads to start; create and start those marker threads (each gets a unique id). After creating them, signal all markers to begin work.
3. Marker threads must wait for the start signal from main before beginning work.
4. Initialize the random number generator using `srand(id)` where `id` is the marker's ordinal number, then begin selecting indices to try to mark.

---

## Key Files
- `Lab 3/main.cpp`
- `Lab 3/Array.h`, `Lab 3/Array.cpp` — shared array wrapper with mutex, iterators, and move semantics.
- `Lab 3/Marker.h`, `Lab 3/Marker.cpp` — marker thread class: `run()`, `unblock()`, `finish()`, bookkeeping.
- `Lab 3/ThreadHandler.h`, `Lab 3/ThreadHandler.cpp` — main-side coordination code that launches markers and interacts with user.
- `tests/` — unit and end-to-end tests for Array, Marker, ThreadHandler, and overall flow.

---

## Build (CMake, Windows)
```bash
cmake -S . -B build -DCMAKE_CXX_STANDARD=20
cmake --build build --config Release
```

---

## Run
Typical interaction:
- Provide array size and number of marker threads.
- Start/stop markers via main thread coordination.
```bash
./build/lab3.exe
```

---

## Testing
```bash
ctest --test-dir build -V
```