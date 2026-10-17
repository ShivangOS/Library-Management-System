#include "Library.h"

int main() {
    LibraryManager lib;
    int mainChoice;

    while (true) {
        std::cout << "\n=== ADVANCED LIBRARY SYSTEM ===\n";
        std::cout << "1. Student Portal\n2. Librarian Portal\n3. Exit\nChoice: ";
        std::cin >> mainChoice;

        if (mainChoice == 1) {
            int studentChoice;
            std::cout << "\n1. Login\n2. Request Account\nChoice: ";
            std::cin >> studentChoice;

            std::string user, pass;
            std::cout << "Username: "; std::cin >> user;
            std::cout << "Password: "; std::cin >> pass;

            if (studentChoice == 2) {
                lib.requestStudentAccount(user, pass);
                std::cout << "Account requested! Please wait for approval.\n";
                continue;
            }

            try {
                if (lib.studentLogin(user, pass)) {
                    int action;
                    while (true) {
                        std::cout << "\n--- Student Menu ---\n";
                        std::cout << "1. View Available Books\n2. View Recently Accessed (LRU Cache)\n3. Issue Book\n4. Return Book\n5. View Account\n6. Logout\nChoice: ";
                        std::cin >> action;

                        try {
                            if (action == 1) lib.viewAvailableBooks();
                            else if (action == 2) lib.viewRecentlyAccessedBooks();
                            else if (action == 3) {
                                std::string id;
                                std::cout << "Enter Book ID to issue: "; std::cin >> id;
                                lib.issueBook(user, id);
                                std::cout << "Book issued successfully!\n";
                            }
                            else if (action == 4) {
                                std::string id;
                                std::cout << "Enter Book ID to return: "; std::cin >> id;
                                lib.returnBook(user, id);
                                std::cout << "Book returned successfully!\n";
                            }
                            else if (action == 5) lib.viewStudentAccount(user);
                            else break;
                        } 
                        catch (const std::exception& e) {
                            std::cerr << "[-] Error: " << e.what() << "\n";
                        }
                    }
                }
            } 
            catch (const std::exception& e) {
                std::cerr << "[-] Error: " << e.what() << "\n";
            }
        } 
        else if (mainChoice == 2) {
            std::string pass;
            std::cout << "Enter Librarian Password: "; std::cin >> pass;

            try {
                if (lib.librarianLogin(pass)) {
                    int action;
                    while (true) {
                        std::cout << "\n--- Librarian Menu ---\n";
                        std::cout << "1. Add Book\n2. Remove Book\n3. View All Borrowed Books\n4. Approve Students\n5. Logout\nChoice: ";
                        std::cin >> action;

                        try {
                            if (action == 1) {
                                std::string id, title, author;
                                std::cout << "ID: "; std::cin >> id;
                                std::cout << "Title: "; std::cin.ignore(); std::getline(std::cin, title);
                                std::cout << "Author: "; std::getline(std::cin, author);
                                lib.addBook(id, title, author);
                                std::cout << "Book added!\n";
                            }
                            else if (action == 2) {
                                std::string id;
                                std::cout << "Enter Book ID to remove: "; std::cin >> id;
                                lib.removeBook(id);
                                std::cout << "Book removed!\n";
                            }
                            else if (action == 3) lib.viewAllBorrowed();
                            else if (action == 4) lib.manageStudentRequests();
                            else break;
                        }
                        catch (const std::exception& e) {
                            std::cerr << "[-] Error: " << e.what() << "\n";
                        }
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "[-] Access Denied: " << e.what() << "\n";
            }
        } 
        else {
            break;
        }
    }
    return 0;
}