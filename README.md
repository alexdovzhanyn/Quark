# Quark

**Quark** is a lightweight, low-level HTTP server built from scratch in C++ using raw TCP sockets. It is designed as an educational project to explore networking fundamentals, HTTP parsing, and systems-level programming with modern C++.

---

## Features

- Basic HTTP/1.1 request parsing
- Support for common HTTP methods (GET, POST, etc.)
- Custom routing via a lightweight router
- Thread-per-connection concurrency model
- Graceful socket management
- Modular design with support for controller-based route handling
- Static library build (`libquark.a`) for integration into other apps

---

## Getting Started

### Build the core library

```bash
make                # builds libquark.a into build/
```

### Build and run the example app

```bash
cd example
make                # builds example_app into ../build/
./build/example_app
```

---

## Example Routes

Inside your app, you can define routes like:

```cpp
Quark::Router::get("/hello", [](HttpRequest &req, HttpResponse &res) {
  res.setStatus(200, "OK");
  res.setBody("Hello, world!");
  res.addHeader("Content-Type", "text/plain");
});
```

or pass a reference to a static controller method:

```cpp
Quark::Router::get("/hello", YourController::someStaticMethod);
```

---

## Tools and Requirements

- C++17 compiler (tested with `clang++` and `g++`)
- POSIX-compatible environment (macOS, Linux)

---

## Future Work

- HTTP keep-alive support
- TLS support (OpenSSL)
- Middleware hooks (e.g., for logging or auth)
- Streaming responses and chunked encoding
- Static file serving with MIME type resolution

