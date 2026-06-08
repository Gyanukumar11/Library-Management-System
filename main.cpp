// ============================================================
//  main.cpp  –  Library Management System
//  Entry point; owns the menu-driven UI loop.
//  Delegates all business logic to the Library facade.
// ============================================================
#include "Library.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ── Forward declarations of sub-menu handlers ────────────────
static void menuBooks(Library& lib);
static void menuStudents(Library& lib);
static void menuIssue(Library& lib);
static void menuReports(Library& lib);

// ── Main Menu ─────────────────────────────────────────────────
static void mainMenu(Library& lib) {
    while (true) {
        Utils::clearScreen();
        Utils::printBanner();
        std::cout
            << "\n  MAIN MENU\n"
            << "  ---------\n"
            << "  1. Book Management\n"
            << "  2. Student Management\n"
            << "  3. Issue / Return Books\n"
            << "  4. Reports & Statistics\n"
            << "  0. Exit\n\n";

        int choice = Utils::readInt("  Choice: ", 0, 4);
        switch (choice) {
            case 1: menuBooks(lib);    break;
            case 2: menuStudents(lib); break;
            case 3: menuIssue(lib);    break;
            case 4: menuReports(lib);  break;
            case 0:
                lib.saveAll();
                std::cout << "\n  Data saved. Goodbye!\n\n";
                return;
        }
    }
}

// ── Book Sub-Menu ─────────────────────────────────────────────
static void menuBooks(Library& lib) {
    while (true) {
        Utils::clearScreen();
        std::cout
            << "\n  BOOK MANAGEMENT\n"
            << "  ---------------\n"
            << "  1. Add Book\n"
            << "  2. Delete Book\n"
            << "  3. Search Book (ID)\n"
            << "  4. Search Book (Title / Author)\n"
            << "  5. Display All Books\n"
            << "  6. Display Available Books\n"
            << "  0. Back\n\n";

        int choice = Utils::readInt("  Choice: ", 0, 6);
        if (choice == 0) return;

        if (choice == 1) {
            // ── Add Book ──────────────────────────────────────
            std::cout << "\n  -- Add New Book --\n";
            int  id     = Utils::readInt("  Book ID     : ");
            auto title  = Utils::readString("  Title       : ");
            auto author = Utils::readString("  Author      : ");
            auto genre  = Utils::readString("  Genre       : ");
            int  copies = Utils::readInt("  Copies      : ");

            try {
                if (lib.addBook(id, title, author, genre, copies))
                    std::cout << "\n  [OK] Book added successfully.\n";
                else
                    std::cout << "\n  [!] Book ID " << id << " already exists.\n";
            } catch (const std::invalid_argument& e) {
                std::cout << "\n  [!] " << e.what() << '\n';
            }

        } else if (choice == 2) {
            // ── Delete Book ───────────────────────────────────
            int id = Utils::readInt("\n  Enter Book ID to delete: ");
            if (lib.deleteBook(id))
                std::cout << "  [OK] Book deleted.\n";
            else
                std::cout << "  [!] Book ID " << id << " not found or cannot be deleted.\n";

        } else if (choice == 3) {
            // ── Search by ID ──────────────────────────────────
            int id = Utils::readInt("\n  Enter Book ID: ");
            const Book* b = lib.findBookById(id);
            if (b) {
                std::cout << '\n';
                std::cout << std::left
                          << std::setw(6)  << "ID"
                          << std::setw(32) << "Title"
                          << std::setw(22) << "Author"
                          << std::setw(16) << "Genre"
                          << std::setw(8)  << "Total"
                          << "Available\n";
                std::cout << std::string(94, '-') << '\n';
                b->display();
            } else {
                std::cout << "  [!] Book ID " << id << " not found.\n";
            }

        } else if (choice == 4) {
            // ── Search by Title / Author ──────────────────────
            auto query   = Utils::readString("\n  Search query: ");
            auto results = lib.findBooksByTitle(query);
            if (results.empty()) {
                std::cout << "  No matching books found.\n";
            } else {
                std::cout << "\n  Found " << results.size() << " result(s):\n";
                std::cout << std::left
                          << std::setw(6)  << "ID"
                          << std::setw(32) << "Title"
                          << std::setw(22) << "Author"
                          << std::setw(16) << "Genre"
                          << std::setw(8)  << "Total"
                          << "Available\n";
                std::cout << std::string(94, '-') << '\n';
                for (const Book* b : results) b->display();
            }

        } else if (choice == 5) {
            std::cout << "\n  -- All Books --\n";
            lib.displayAllBooks();

        } else if (choice == 6) {
            std::cout << "\n  -- Available Books --\n";
            lib.displayAvailableBooks();
        }

        Utils::pause();
    }
}

// ── Student Sub-Menu ─────────────────────────────────────────
static void menuStudents(Library& lib) {
    while (true) {
        Utils::clearScreen();
        std::cout
            << "\n  STUDENT MANAGEMENT\n"
            << "  ------------------\n"
            << "  1. Register Student\n"
            << "  2. Find Student by ID\n"
            << "  3. Display All Students\n"
            << "  0. Back\n\n";

        int choice = Utils::readInt("  Choice: ", 0, 3);
        if (choice == 0) return;

        if (choice == 1) {
            // ── Register ──────────────────────────────────────
            std::cout << "\n  -- Register New Student --\n";
            int  id     = Utils::readInt("  Student ID : ");
            auto name   = Utils::readString("  Name       : ");

            std::string email;
            while (true) {
                email = Utils::readString("  Email      : ");
                if (Utils::isValidEmail(email)) break;
                std::cout << "  [!] Invalid email format. Please try again.\n";
            }
            auto course = Utils::readString("  Course     : ");

            try {
                if (lib.registerStudent(id, name, email, course))
                    std::cout << "\n  [OK] Student registered.\n";
                else
                    std::cout << "\n  [!] Student ID " << id << " already exists.\n";
            } catch (const std::invalid_argument& e) {
                std::cout << "\n  [!] " << e.what() << '\n';
            }

        } else if (choice == 2) {
            int id = Utils::readInt("\n  Enter Student ID: ");
            const Student* s = lib.findStudentById(id);
            if (s) {
                s->display();
                s->displayIssuedBooks();
            } else {
                std::cout << "  [!] Student ID " << id << " not found.\n";
            }

        } else if (choice == 3) {
            std::cout << "\n  -- All Students --\n";
            lib.displayAllStudents();
        }

        Utils::pause();
    }
}

// ── Issue / Return Sub-Menu ───────────────────────────────────
static void menuIssue(Library& lib) {
    while (true) {
        Utils::clearScreen();
        std::cout
            << "\n  ISSUE / RETURN BOOKS\n"
            << "  --------------------\n"
            << "  1. Issue Book to Student\n"
            << "  2. Return Book from Student\n"
            << "  3. View All Issued Books\n"
            << "  0. Back\n\n";

        int choice = Utils::readInt("  Choice: ", 0, 3);
        if (choice == 0) return;

        if (choice == 1) {
            std::cout << "\n  -- Issue Book --\n";
            int sid = Utils::readInt("  Student ID : ");
            int bid = Utils::readInt("  Book ID    : ");
            if (lib.issueBook(sid, bid))
                std::cout << "  [OK] Book issued successfully.\n";

        } else if (choice == 2) {
            std::cout << "\n  -- Return Book --\n";
            int sid = Utils::readInt("  Student ID : ");
            int bid = Utils::readInt("  Book ID    : ");
            if (lib.returnBook(sid, bid))
                std::cout << "  [OK] Book returned successfully.\n";

        } else if (choice == 3) {
            lib.displayIssuedBooks();
        }

        Utils::pause();
    }
}

// ── Reports Sub-Menu ─────────────────────────────────────────
static void menuReports(Library& lib) {
    while (true) {
        Utils::clearScreen();
        std::cout
            << "\n  REPORTS & STATISTICS\n"
            << "  --------------------\n"
            << "  1. Statistics Dashboard\n"
            << "  2. All Books\n"
            << "  3. Available Books\n"
            << "  4. All Issued Books\n"
            << "  5. All Students\n"
            << "  0. Back\n\n";

        int choice = Utils::readInt("  Choice: ", 0, 5);
        if (choice == 0) return;

        switch (choice) {
            case 1: lib.displayStatistics();    break;
            case 2: lib.displayAllBooks();      break;
            case 3: lib.displayAvailableBooks();break;
            case 4: lib.displayIssuedBooks();   break;
            case 5: lib.displayAllStudents();   break;
        }
        Utils::pause();
    }
}

// ── Entry Point ───────────────────────────────────────────────
int main() {
    try {
        Library lib("books.dat", "students.dat");
        mainMenu(lib);
    } catch (const std::exception& ex) {
        std::cerr << "\n  [FATAL] " << ex.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
