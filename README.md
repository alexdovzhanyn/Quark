# Quark

**Quark** is a lightweight, low-level HTTP server built from scratch in C++ using raw TCP sockets. It is designed as an educational project to explore networking fundamentals, HTTP parsing, and systems-level programming with modern C++.

---

## Features

- Basic HTTP/1.1 request parsing
- Support for common HTTP methods (GET, POST, etc.)
- Custom routing via a lightweight router
- Serving HTML and other file types from custom routes
- Static file serving with MIME type resolution
- Request + response middleware hooks
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
Quark::Router::GET("/hello", [](Quark::HttpRequest &req) {
  return Quark::HttpResponse::ok()
    .setBody("Hello, world!")
    .addHeader("Content-Type", "text/plain");
});
```

or pass a reference to a static controller method:

```cpp
Quark::Router::GET("/hello", YourController::someStaticMethod);
```

Your routes can also return files!

```cpp
Quark::Router::GET("/cat", [](Quark::HttpRequest &req) {
  return Quark::HttpResponse::sendFile("path/to/kitty.png");
});
```

or

```cpp
Quark::Router::GET("/home", [](Quark::HttpRequest &res) {
  return Quark::HttpResponse::sendFile("public/index.html");
})
```

---

## Static File Serving

By default, Quark will serve static files from a `public/` directory contained
in the root of your project. Requests to the server such as /img/cat.png
will be routed to /public/img/cat.png if the file exists.

Quark can also be configured to serve static files from a different directory, if needed:

```cpp
Quark::Server server = Quark::Server("9669");

server.staticServe("images");
```

## Custom Middleware

Often times, it is necessary to parse the body or augment a request / response. For this purpose,
you can register middleware functions with the Quark server, which will run in the order they are
defined.

```cpp
Quark::Server server = Quark::Server("9669");

server
  .registerRequestMiddleware([](Quark::HttpRequest& request) {
    request.setBody("Overwritten body!");
  })
  .registerResponseMiddleware([](Quark::HttpResponse& response) {
    response.addHeader("Access-Control-Allow-Origin", "*");
  });

server.run();
```

## Tools and Requirements

- C++17 compiler (tested with `clang++` and `g++`)
- POSIX-compatible environment (macOS, Linux)

---

## Future Work

- HTTP keep-alive support
- TLS support (OpenSSL)
- Streaming responses and chunked encoding
