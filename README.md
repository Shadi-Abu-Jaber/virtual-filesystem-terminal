## Virtual Filesystem & Mini Terminal

This repository contains my solution to HW1 from the **Advanced Topics in OOP** course.  
The goal of the assignment was to design and implement a small **virtual file system** with a simple, terminal‑like interface in C++.

### What this project does
- Models a file system in memory using C++ classes.
- Lets you:
  - Create and delete virtual files and directories.
  - Navigate between directories.
  - List directory contents and print basic information.
- Handles invalid commands and paths with simple error reporting.

### Design highlights
- **Language**: C++
- **OOP ideas used**:
  - Inheritance and polymorphism for files vs. directories.
  - Encapsulation of file system logic in dedicated classes.
  - A `Terminal` class that parses user commands and delegates to the virtual file system.
- **Main components**:
  - `VFile`, `VDirectory`, `VFileSystem` – in‑memory representation of the hierarchy.
  - `Terminal` – command loop and user interaction.
  - `main.cpp` – creates the terminal and starts the loop.

