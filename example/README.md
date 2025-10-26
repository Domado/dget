# Compile and Run

This project contains a C library (`libdaisohttp`) and two test programs:

1. A simple command-line client written in C (`main.c`).

2. A unit test program written in C++ (`test.cpp`).

## Dependencies
* `make`
* A C compiler (e.g., `gcc` or `clang`)
* A C++ compiler (e.g., `g++` or `clang++`)

---

## Compile and Run the C Test Client (Makefile)

This is the primary and simplest test method. `Makefile` is configured to compile the C library and link `main.c`.

### 1. Compile

In the project root directory, run `make` directly:

```bash
make
```
### 2. GCC

Compile and link the C++ test program: Use g++ (the C++ compiler) and link the C library libdaisohttp.a.

```bash
g++ -Wall -Wextra -std=c++11 -I. test.cpp -o daiso_test libdaisohttp.a
```

### 3. Run

```bash
./daiso_test
```
---
