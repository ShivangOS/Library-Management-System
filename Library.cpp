#include "Library.h"

LibraryManager::LibraryManager() : cacheCapacity(5) {
    loadBooks();
    loadStudents();
    loadBorrows();
}

LibraryManager::~LibraryManager() {
    saveBooks();
    saveStudents();
    saveBorrows();
}

// --- LRU ALGORITHM ---
void LibraryManager::updateLRU(std::shared_ptr<Book> book) {
    if (lruIterMap.find(book->id) != lruIterMap.end()) {
        lruList.erase(lruIterMap[book->id]);
    } 
    else if (lruList.size() >= cacheCapacity) {
        std::string last = lruList.back();
        lruList.pop_back();
        lruIterMap.erase(last);
        lruCacheMap.erase(last);
    }
    lruList.push_front(book->id);
    lruIterMap[book->id] = lruList.begin();
    lruCacheMap[book->id] = book;
}

// --- FILE I/O ---
void LibraryManager::loadBooks() {
    std::ifstream file("books.txt");
    std::string line, id, title, author, availStr;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, author, ',');
        std::getline(ss, availStr, ',');
        books.push_back(std::make_shared<Book>(Book{id, title, author, availStr == "1"}));
    }
}

void LibraryManager::saveBooks() {
    std::ofstream file("books.txt");
    for (const auto& b : books) {
        file << b->id << "," << b->title << "," << b->author << "," << (b->isAvailable ? "1" : "0") << "\n";
    }
}

void LibraryManager::loadStudents() {
    std::ifstream file("students.txt");
    std::string line, user, pass, appStr, fineStr;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, user, ',');
        std::getline(ss, pass, ',');
        std::getline(ss, appStr, ',');
        std::getline(ss, fineStr, ',');
        students.push_back({user, pass, appStr == "1", std::stoi(fineStr)});
    }
}

void LibraryManager::saveStudents() {
    std::ofstream file("students.txt");
    for (const auto& s : students) {
        file << s.username << "," << s.password << "," << (s.isApproved ? "1" : "0") << "," << s.fineAmount << "\n";
    }
}

void LibraryManager::loadBorrows() {
    std::ifstream file("borrows.txt");
    std::string line, user, bId, retStr;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::getline(ss, user, ',');
        std::getline(ss, bId, ',');
        std::getline(ss, retStr, ',');
        borrows.push_back({user, bId, retStr == "1"});
    }
}

void LibraryManager::saveBorrows() {
    std::ofstream file("borrows.txt");
    for (const auto& br : borrows) {
        file << br.username << "," << br.bookId << "," << (br.isReturned ? "1" : "0") << "\n";
    }
}

// --- STUDENT LOGIC ---
bool LibraryManager::studentLogin(std::string username, std::string password) {
    std::lock_guard<std::mutex> lock(dbMutex); 
    for (const auto& s : students) {
        if (s.username == username && s.password == password) {
            if (s.isApproved) return true;
        }
    }
    throw AuthException();
}

void LibraryManager::requestStudentAccount(std::string username, std::string password) {
    std::lock_guard<std::mutex> lock(dbMutex);
    students.push_back({username, password, false, 0});
    saveStudents();
}

void LibraryManager::viewAvailableBooks() {
    std::lock_guard<std::mutex> lock(dbMutex);
    std::cout << "\n--- Available Books ---\n";
    for (const auto& b : books) {
        if (b->isAvailable) {
            std::cout << "ID: " << b->id << " | " << b->title << " by " << b->author << "\n";
            updateLRU(b);
        }
    }
}

void LibraryManager::viewRecentlyAccessedBooks() {
    std::lock_guard<std::mutex> lock(dbMutex);
    std::cout << "\n--- Recently Viewed/Accessed Books (LRU Cache) ---\n";
    if (lruList.empty()) std::cout << "No recent activity.\n";
    for (const std::string& id : lruList) {
        auto b = lruCacheMap[id];
        std::cout << "ID: " << b->id << " | " << b->title << " (Available: " << (b->isAvailable ? "Yes" : "No") << ")\n";
    }
}

void LibraryManager::issueBook(std::string username, std::string bookId) {
    std::lock_guard<std::mutex> lock(dbMutex);
    for (auto& b : books) {
        if (b->id == bookId && b->isAvailable) {
            b->isAvailable = false;
            borrows.push_back({username, bookId, false});
            updateLRU(b);
            saveBooks();
            saveBorrows();
            return; 
        }
    }
    throw BookNotFoundException();
}

void LibraryManager::returnBook(std::string username, std::string bookId) {
    std::lock_guard<std::mutex> lock(dbMutex);
    for (auto& br : borrows) {
        if (br.username == username && br.bookId == bookId && !br.isReturned) {
            br.isReturned = true;
            for (auto& b : books) {
                if (b->id == bookId) {
                    b->isAvailable = true;
                    updateLRU(b);  
                }
            }
            saveBooks();
            saveBorrows();
            return;
        }
    }
    throw BookNotFoundException();
}

void LibraryManager::viewStudentAccount(std::string username) {
    std::lock_guard<std::mutex> lock(dbMutex);
    std::cout << "\n--- My Account ---\n";
    for (const auto& s : students) {
        if (s.username == username) {
            std::cout << "Current Fines: $" << s.fineAmount << "\n";
            break;
        }
    }
    std::cout << "Borrow History:\n";
    for (const auto& br : borrows) {
        if (br.username == username) {
            std::cout << "Book ID: " << br.bookId << " | Status: " << (br.isReturned ? "Returned" : "Borrowing") << "\n";
        }
    }
}

// --- LIBRARIAN LOGIC ---
bool LibraryManager::librarianLogin(std::string password) {
    if (password != "admin123") throw AuthException();
    return true; 
}

void LibraryManager::addBook(std::string id, std::string title, std::string author) {
    std::lock_guard<std::mutex> lock(dbMutex);
    books.push_back(std::make_shared<Book>(Book{id, title, author, true}));
    saveBooks();
}

void LibraryManager::removeBook(std::string id) {
    std::lock_guard<std::mutex> lock(dbMutex);
    for (auto it = books.begin(); it != books.end(); ++it) {
        if ((*it)->id == id) {
            books.erase(it);
            saveBooks();
            return;
        }
    }
    throw BookNotFoundException();
}

void LibraryManager::viewAllBorrowed() {
    std::lock_guard<std::mutex> lock(dbMutex);
    std::cout << "\n--- All Borrowed Books ---\n";
    for (const auto& br : borrows) {
        if (!br.isReturned) {
            std::cout << "User: " << br.username << " | Book ID: " << br.bookId << "\n";
        }
    }
}

void LibraryManager::manageStudentRequests() {
    std::lock_guard<std::mutex> lock(dbMutex);
    for (auto& s : students) {
        if (!s.isApproved) {
            std::cout << "Approve student '" << s.username << "'? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') s.isApproved = true;
        }
    }
    saveStudents();
}