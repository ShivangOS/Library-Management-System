# C++ Library Management System

A robust, terminal-based Library Management System built in standard C++. This project goes beyond basic CRUD operations by implementing enterprise-level software patterns, including thread safety, memory management via smart pointers, and a custom Least Recently Used (LRU) cache algorithm for book tracking.

Data is persistently stored using simple `.txt` files, making it lightweight and easy to deploy without requiring an SQL database.

---

## Tech Stack & Prerequisites

- **Language:** C++14 (or newer)
- **Compiler:** GCC/MinGW (or any standard C++ compiler)
- **Libraries Used:** `<iostream>`, `<vector>`, `<string>`, `<fstream>`, `<mutex>`, `<memory>`, `<exception>`, `<list>`, `<unordered_map>`
- **Dependencies:** None (uses only the C++ Standard Library)

---

## Project Structure

Ensure your project folder contains the following files:

```text
Library.h
Library.cpp
main.cpp
books.txt
students.txt
borrows.txt
```

### File Descriptions

| File | Purpose |
|--------|---------|
| `Library.h` | Class declarations and interfaces |
| `Library.cpp` | Core business logic and database management |
| `main.cpp` | Terminal-based user interface |
| `books.txt` | Book inventory database |
| `students.txt` | Student account database |
| `borrows.txt` | Borrow/return transaction records |

---

## How to Compile and Run

### Compile

Open PowerShell or Command Prompt in the project directory and run:

```powershell
g++ -std=c++14 -Wall main.cpp Library.cpp -o library_app.exe
```

### Run

```powershell
./library_app.exe
```

---

## Test Credentials

Use the following default credentials for testing (assuming the provided seed files are present):

### Librarian Account

- Password: `admin123`

### Approved Student Account

- Username: `user`
- Password: `1234`

### Unapproved Student Account

- Username: `guest`
- Password: `1234`

---

## Core Concepts Demonstrated

### 1. Multithreading & Concurrency (`std::mutex`)

To prevent data corruption when multiple users access the database simultaneously, the `LibraryManager` uses a `std::mutex`.

All critical read/write operations are protected using:

```cpp
std::lock_guard<std::mutex> lock(dbMutex);
```

---

### 2. Smart Memory Management (`std::shared_ptr`)

The project avoids raw pointers and manual memory management.

Books are stored using:

```cpp
std::shared_ptr<Book>
```
---

### 3. LRU Cache Algorithm (Least Recently Used)

The project includes a "Recently Accessed Books" feature implemented using an O(1) LRU Cache.

#### Data Structures Used

- `std::list`
- `std::unordered_map`

#### Workflow

1. Accessed books move to the front of the list.
2. The hash map provides O(1) lookup.
3. When cache capacity is exceeded, the least recently used book is removed.
---

### 4. Custom Exception Handling

Instead of relying solely on boolean return values, the system uses custom exceptions.


### 5. File I/O Persistence

The application stores data in text files.


### 6. Object-Oriented Programming (OOP)

The system follows strong OOP principles.

#### Encapsulation

Sensitive data structures remain private inside the `LibraryManager` class.

#### Abstraction

The UI interacts only through public methods without knowing internal implementation details.


---

## Author
- Shivang Dixit