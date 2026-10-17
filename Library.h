#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <mutex>
#include <memory>
#include <exception>
#include <list>
#include <unordered_map>

// Custom Exceptions
class AuthException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Authentication failed: Invalid credentials or unapproved account.";
    }
};

class BookNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Operation failed: Book not found or unavailable.";
    }
};

// Data Structures
struct Book {
    std::string id;
    std::string title;
    std::string author;
    bool isAvailable;
};

struct Student {
    std::string username;
    std::string password;
    bool isApproved;
    int fineAmount;
};

struct BorrowRecord {
    std::string username;
    std::string bookId;
    bool isReturned;
};

// Main Library Manager Class
class LibraryManager {
private:
    std::vector<std::shared_ptr<Book>> books; 
    std::vector<Student> students;
    std::vector<BorrowRecord> borrows;

    std::mutex dbMutex; 

    // LRU Cache specific members
    size_t cacheCapacity;
    std::list<std::string> lruList; 
    std::unordered_map<std::string, std::list<std::string>::iterator> lruIterMap; 
    std::unordered_map<std::string, std::shared_ptr<Book>> lruCacheMap; 
    void updateLRU(std::shared_ptr<Book> book);

    // File I/O Helpers
    void loadBooks();
    void saveBooks();
    void loadStudents();
    void saveStudents();
    void loadBorrows();
    void saveBorrows();

public:
    LibraryManager();
    ~LibraryManager();

    // Student Functions
    bool studentLogin(std::string username, std::string password);
    void requestStudentAccount(std::string username, std::string password);
    void viewAvailableBooks();
    void viewRecentlyAccessedBooks(); // LRU Feature
    void issueBook(std::string username, std::string bookId);
    void returnBook(std::string username, std::string bookId);
    void viewStudentAccount(std::string username);

    // Librarian Functions
    bool librarianLogin(std::string password);
    void addBook(std::string id, std::string title, std::string author);
    void removeBook(std::string id);
    void viewAllBorrowed();
    void manageStudentRequests();
};

#endif