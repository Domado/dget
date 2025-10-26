# dget (Daiso GET)

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/Domado/dcp/actions)
[![License](https://img.shields.io/badge/license-MIT-blue)](https://opensource.org/licenses/MIT)
![Version](https://img.shields.io/badge/version-0.1.0-informational)

👍🏻 Powerful, flexible, and reliable command-line tool for downloading files over HTTP, made by Daiso

`dget` is a minimal, educational HTTP GET client and foundational C library (`libdaisohttp`) written from scratch using standard C and POSIX socket APIs. It serves as the foundational networking component for the **Daiso Engine** project.

Its primary goal is to provide a clear, modular, and extensible C codebase for understanding how HTTP clients work at the lowest level, before introducing complex dependencies like OpenSSL or libcurl.

---

## Features

* **Modular, Library-First Design:** The core logic is a static library (`libdaisohttp.a`) with a clean public API (`daiso_http.h`). The `dget` client is just a simple consumer of this library.
* **Modern DNS Resolution:** Uses `getaddrinfo()` for protocol-agnostic (IPv4/IPv6) hostname resolution.
* **Dynamic Response Buffering:** `daiso_net_recv_dynamic()` automatically allocates and resizes a buffer to receive the entire HTTP response, avoiding fixed-size buffer overflows.
* **Clean API:** A simple `Handle`-based API (`init`, `set_url`, `perform`, `cleanup`).
* **Basic HTTP Parsing:** Parses the HTTP status line and separates headers from the body.
* **Portable C Code:** Written in standard C99 with no external dependencies.

## Current Limitations

This is a foundational library. To "beat wget," it would need to overcome these limitations:

* **NO HTTPS (SSL/TLS):** This is the biggest limitation. It only supports plain `http://` on port 80.
* **HTTP/1.0 Only:** It uses `HTTP/1.0` and `Connection: close`, meaning it opens a new TCP connection for every request. It does **not** support HTTP/1.1 Keep-Alive or Chunked Transfer Encoding.
* **No Redirects:** It does not automatically follow 301/302 redirects.
* **Basic URL Parser:** The current parser only handles simple `http://host/path` formats. It does not support usernames, passwords, or custom ports in the URL.
* **Blocking I/O:** All network operations (`connect`, `send`, `recv`) are synchronous and blocking.

---

## How to Build

A `Makefile` is provided. The only requirement is a C compiler (like `gcc` or `clang`) and `make`.

1.  **Save all `.c` and `.h` files** provided, along with the `Makefile`.

2.  **Compile the project:**

    ```bash
    make
    ```

    This will:
    * Compile all library object files (`.o`).
    * Archive them into a static library `libdaisohttp.a`.
    * Compile `main.o`.
    * Link `main.o` against `libdaisohttp.a` to create the final `dget` (or `http_client_test`) executable.

4.  **Clean up build files:**

    ```bash
    make clean
    ```

## Usage

You can run the compiled client from your terminal. It takes one argument: the full URL (which must start with `http://`).

```bash
# Syntax
./dget <url>

# Example
./dget [http://example.com/](http://example.com/)
