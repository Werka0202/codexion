# Codexion: Resource Scheduling & Concurrency Simulator [WIP]

Codexion is a multi-threaded simulation project written in C, focusing on concurrency, resource allocation, and preventing deadlocks. It simulates an environment where multiple entities (coders) must share limited resources (dongles) to execute tasks (compiling, debugging, refactoring) without crashing or burning out.

### Technical Highlights
* **Multithreading**: Extensive use of `pthread` to manage concurrent execution.
* **Resource Protection**: Implementing `pthread_mutex` to handle shared resources safely and prevent race conditions.
* **Scheduling Algorithms**: Built-in support for different scheduling approaches (FIFO / EDF) to manage task execution order efficiently.

### Current Status
This project is currently under active development. The initialization logic, argument parsing, and data structures are set up. Work is ongoing on the main thread routines and the monitoring system.

### How to Build (Development)
```bash
make
./codexion [num_coders] [burnout] [compile] [debug] [refactor] [req_compiles] [cooldown] [fifo/edf]
