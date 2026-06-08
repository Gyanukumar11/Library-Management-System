# Library-Management-System
A C++ Library Management System built using Object-Oriented Programming and file handling, supporting book management, issue/return operations, member records, and persistent data storage.
# Library Management System
### A Production-Quality C++17 Console Application

---

## Overview

The **Library Management System (LMS)** is a fully featured, menu-driven console application
written in modern **C++17**. It demonstrates advanced Object-Oriented Programming,
STL usage, file persistence, exception handling, and clean modular architecture —
making it an ideal portfolio/resume project for software engineering roles.

---

## Features

| Feature | Description |
|---|---|
| Add / Delete Books | Add new books with unique IDs; safe delete (blocked if copies are issued) |
| Search Books | Find by exact ID or fuzzy title/author match (case-insensitive) |
| Register Students | Register library members with email validation |
| Issue Book | Allocate a copy to a student; tracks per-student and per-book counts |
| Return Book | Reclaim a copy; validates the student actually holds that book |
| Available Copies | Real-time tracking of total vs available vs issued copies |
| Prevent Duplicate IDs | Both book and student IDs are uniqueness-enforced at insertion time |
| File Persistence | All data auto-saved on exit and auto-loaded on startup (`books.dat`, `students.dat`) |
| Display Issued Books | Full audit trail of which students hold which books |
| Statistics Dashboard | Snapshot of total titles, total copies, issued copies, available copies, students |

---

## Project Structure

```
LibraryManagementSystem/
├── Book.h          # Book class declaration
├── Book.cpp        # Book class implementation
├── Student.h       # Student class declaration
├── Student.cpp     # Student class implementation
├── Library.h       # Library facade declaration
├── Library.cpp     # Library facade implementation
├── Utils.h         # Input validation & UI helpers (header-only)
├── main.cpp        # Entry point + menu-driven UI
├── Makefile        # Linux/macOS build
└── README.md       # This file
```

---

## OOP Design

```
Book          (encapsulated entity)
  ├─ serialise / deserialise   (file I/O abstraction)
  └─ issueBook / returnBook    (copy-count logic)

Student       (encapsulated entity)
  ├─ owns vector<int> issuedBooks
  └─ serialise / deserialise

Library       (facade / controller)
  ├─ owns map<int, Book>     (O(log n) lookup)
  ├─ owns map<int, Student>  (O(log n) lookup)
  ├─ mediates Issue/Return workflow
  └─ delegates file I/O to Book/Student
```

**OOP Principles demonstrated:**
- **Encapsulation** – all data members are private; access only via getters/setters
- **Abstraction** – `Library` exposes a clean API; callers never touch raw maps
- **Polymorphism** – virtual-dispatch-ready design; `Utils` namespace provides overloaded helpers
- **Separation of Concerns** – entity classes, controller class, UI layer, and utilities are fully separate

---

## Compilation

### Linux / macOS

```bash
# Using Makefile
make
./lms

# Or manually
g++ -std=c++17 -Wall -Wextra -O2 -o lms main.cpp Book.cpp Student.cpp Library.cpp
./lms
```

### Windows (MinGW / MSYS2)

```cmd
g++ -std=c++17 -Wall -Wextra -O2 -o lms.exe main.cpp Book.cpp Student.cpp Library.cpp
lms.exe
```

### Windows (MSVC — Developer Command Prompt)

```cmd
cl /std:c++17 /EHsc /O2 /Fe:lms.exe main.cpp Book.cpp Student.cpp Library.cpp
lms.exe
```

### Clean build

```bash
make clean
```

> **Requires:** GCC ≥ 7, Clang ≥ 5, or MSVC 2017+ with C++17 support.

---

## Data Files

| File | Contents |
|---|---|
| `books.dat` | Pipe-delimited book records (auto-created on first run) |
| `students.dat` | Pipe-delimited student records (auto-created on first run) |

Both files are human-readable and auto-saved when the program exits cleanly.
On re-launch the system restores the complete previous state.

---

## Sample Session

```
 ╔══════════════════════════════════════════════════╗
 ║          LIBRARY MANAGEMENT SYSTEM               ║
 ║              C++17 Edition                       ║
 ╚══════════════════════════════════════════════════╝

  MAIN MENU
  ---------
  1. Book Management
  2. Student Management
  3. Issue / Return Books
  4. Reports & Statistics
  0. Exit

  Choice: 1

  BOOK MANAGEMENT
  ...
  Choice: 1
  Book ID     : 101
  Title       : The Great Gatsby
  Author      : F. Scott Fitzgerald
  Genre       : Fiction
  Copies      : 3
  [OK] Book added successfully.

  Choice: 0

  MAIN MENU > 4 > 1  (Statistics Dashboard)
--------------------------------------------
      LIBRARY STATISTICS DASHBOARD
--------------------------------------------
  Distinct Book Titles:       2
  Total Copies (all books):   8
  Copies Currently Issued:    1
  Copies Available:           7
  Registered Students:        1
--------------------------------------------
```

---

## Technical Highlights

- **C++17** structured bindings (`for (auto& [id, book] : m_books)`)
- **`std::map`** for O(log n) lookup by integer key
- **`std::vector<int>`** within `Student` to track issued book IDs
- **STL algorithms** — `std::find`, `std::transform` for case-insensitive search
- **RAII** — `Library` destructor auto-saves data (`saveAll()` in `~Library()`)
- **Exception safety** — constructors throw `std::invalid_argument` on bad input; `main` catches `std::exception`
- **Input validation** — `Utils::readInt` and `Utils::readString` loop until valid input is received; no raw `cin >>` in business logic
- **No global variables** — all state owned by `Library`, passed by reference to menu functions

---

## License

MIT — free for personal, academic, and commercial use.
