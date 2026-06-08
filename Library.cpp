#include "Library.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cctype>

// ─────────────────────────────────────────────────────────────
//  Utility: case-insensitive substring search
// ─────────────────────────────────────────────────────────────
static bool containsCI(const std::string& haystack, const std::string& needle) {
    if (needle.empty()) return true;
    auto toLower = [](unsigned char c){ return std::tolower(c); };
    std::string h = haystack, n = needle;
    std::transform(h.begin(), h.end(), h.begin(), toLower);
    std::transform(n.begin(), n.end(), n.begin(), toLower);
    return h.find(n) != std::string::npos;
}

// ── Construction / Destruction ───────────────────────────────
Library::Library(const std::string& bookFile, const std::string& studentFile)
    : m_bookFile(bookFile), m_studentFile(studentFile)
{
    loadAll();
}

Library::~Library() {
    saveAll();
}

// ── Book Management ───────────────────────────────────────────
bool Library::addBook(int id, const std::string& title, const std::string& author,
                      const std::string& genre, int copies)
{
    if (m_books.count(id)) return false;   // duplicate ID
    m_books.emplace(id, Book(id, title, author, genre, copies));
    return true;
}

bool Library::deleteBook(int id) {
    auto it = m_books.find(id);
    if (it == m_books.end()) return false;

    // Prevent deletion if any copy is currently issued
    const Book& b = it->second;
    if (b.getAvailableCopies() < b.getTotalCopies()) {
        std::cout << "  [!] Cannot delete – one or more copies are currently issued.\n";
        return false;
    }
    m_books.erase(it);
    return true;
}

Book* Library::findBookById(int id) {
    auto it = m_books.find(id);
    return (it != m_books.end()) ? &it->second : nullptr;
}

const Book* Library::findBookById(int id) const {
    auto it = m_books.find(id);
    return (it != m_books.end()) ? &it->second : nullptr;
}

std::vector<const Book*> Library::findBooksByTitle(const std::string& query) const {
    std::vector<const Book*> results;
    for (const auto& [id, book] : m_books) {
        if (containsCI(book.getTitle(), query) ||
            containsCI(book.getAuthor(), query))
        {
            results.push_back(&book);
        }
    }
    return results;
}

void Library::displayAllBooks() const {
    if (m_books.empty()) {
        std::cout << "  No books in the library.\n";
        return;
    }
    printBookHeader();
    for (const auto& [id, book] : m_books)
        book.display();
    printDivider(94);
}

void Library::displayAvailableBooks() const {
    bool found = false;
    printBookHeader();
    for (const auto& [id, book] : m_books) {
        if (book.getAvailableCopies() > 0) {
            book.display();
            found = true;
        }
    }
    if (!found) std::cout << "  No books currently available.\n";
    printDivider(94);
}

// ── Student Management ────────────────────────────────────────
bool Library::registerStudent(int id, const std::string& name,
                               const std::string& email, const std::string& course)
{
    if (m_students.count(id)) return false;
    m_students.emplace(id, Student(id, name, email, course));
    return true;
}

Student* Library::findStudentById(int id) {
    auto it = m_students.find(id);
    return (it != m_students.end()) ? &it->second : nullptr;
}

const Student* Library::findStudentById(int id) const {
    auto it = m_students.find(id);
    return (it != m_students.end()) ? &it->second : nullptr;
}

void Library::displayAllStudents() const {
    if (m_students.empty()) {
        std::cout << "  No students registered.\n";
        return;
    }
    printStudentHeader();
    for (const auto& [id, st] : m_students)
        st.display();
    printDivider(88);
}

// ── Issue / Return ─────────────────────────────────────────────
bool Library::issueBook(int studentId, int bookId) {
    Student* student = findStudentById(studentId);
    if (!student) {
        std::cout << "  [!] Student ID " << studentId << " not found.\n";
        return false;
    }

    Book* book = findBookById(bookId);
    if (!book) {
        std::cout << "  [!] Book ID " << bookId << " not found.\n";
        return false;
    }

    if (student->hasBook(bookId)) {
        std::cout << "  [!] Student already holds a copy of this book.\n";
        return false;
    }

    if (!book->issueBook()) {
        std::cout << "  [!] No available copies of \"" << book->getTitle() << "\".\n";
        return false;
    }

    student->addIssuedBook(bookId);
    return true;
}

bool Library::returnBook(int studentId, int bookId) {
    Student* student = findStudentById(studentId);
    if (!student) {
        std::cout << "  [!] Student ID " << studentId << " not found.\n";
        return false;
    }

    Book* book = findBookById(bookId);
    if (!book) {
        std::cout << "  [!] Book ID " << bookId << " not found.\n";
        return false;
    }

    if (!student->removeIssuedBook(bookId)) {
        std::cout << "  [!] This student does not currently hold Book ID " << bookId << ".\n";
        return false;
    }

    book->returnBook();
    return true;
}

void Library::displayIssuedBooks() const {
    bool anyIssued = false;
    std::cout << "\n  ── Currently Issued Books ──────────────────────────────\n";
    for (const auto& [sid, student] : m_students) {
        if (student.issuedCount() > 0) {
            student.displayIssuedBooks();
            // Print titles alongside IDs for readability
            for (int bid : student.getIssuedBooks()) {
                const Book* b = findBookById(bid);
                if (b)
                    std::cout << "     Book ID " << bid
                              << " : \"" << b->getTitle() << "\"\n";
            }
            anyIssued = true;
        }
    }
    if (!anyIssued)
        std::cout << "  No books are currently issued.\n";
    std::cout << '\n';
}

// ── Statistics ─────────────────────────────────────────────────
void Library::displayStatistics() const {
    int totalBooks     = 0;
    int totalCopies    = 0;
    int issuedCopies   = 0;
    int availCopies    = 0;

    for (const auto& [id, book] : m_books) {
        ++totalBooks;
        totalCopies  += book.getTotalCopies();
        availCopies  += book.getAvailableCopies();
        issuedCopies += book.getTotalCopies() - book.getAvailableCopies();
    }

    int totalStudents = static_cast<int>(m_students.size());

    std::cout << "\n";
    printDivider(44);
    std::cout << "      LIBRARY STATISTICS DASHBOARD\n";
    printDivider(44);
    std::cout << std::left
              << "  " << std::setw(28) << "Distinct Book Titles:"
              << totalBooks    << '\n'
              << "  " << std::setw(28) << "Total Copies (all books):"
              << totalCopies   << '\n'
              << "  " << std::setw(28) << "Copies Currently Issued:"
              << issuedCopies  << '\n'
              << "  " << std::setw(28) << "Copies Available:"
              << availCopies   << '\n'
              << "  " << std::setw(28) << "Registered Students:"
              << totalStudents << '\n';
    printDivider(44);
    std::cout << '\n';
}

// ── Persistence ───────────────────────────────────────────────
void Library::saveAll() const {
    // Save books
    {
        std::ofstream out(m_bookFile);
        if (!out)
            throw std::runtime_error("Cannot open " + m_bookFile + " for writing.");
        out << m_books.size() << '\n';
        for (const auto& [id, book] : m_books)
            book.serialise(out);
    }

    // Save students
    {
        std::ofstream out(m_studentFile);
        if (!out)
            throw std::runtime_error("Cannot open " + m_studentFile + " for writing.");
        out << m_students.size() << '\n';
        for (const auto& [id, student] : m_students)
            student.serialise(out);
    }
}

void Library::loadAll() {
    // Load books
    {
        std::ifstream in(m_bookFile);
        if (in) {
            size_t count = 0;
            in >> count;
            in.ignore();   // consume newline after count
            for (size_t i = 0; i < count; ++i) {
                Book b;
                b.deserialise(in);
                m_books[b.getId()] = std::move(b);
            }
        }
    }

    // Load students
    {
        std::ifstream in(m_studentFile);
        if (in) {
            size_t count = 0;
            in >> count;
            in.ignore();
            for (size_t i = 0; i < count; ++i) {
                Student s;
                s.deserialise(in);
                m_students[s.getId()] = std::move(s);
            }
        }
    }
}

// ── Private Helpers ───────────────────────────────────────────
void Library::printBookHeader() const {
    printDivider(94);
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(32) << "Title"
              << std::setw(22) << "Author"
              << std::setw(16) << "Genre"
              << std::setw(8)  << "Total"
              << std::setw(10) << "Available"
              << '\n';
    printDivider(94);
}

void Library::printStudentHeader() const {
    printDivider(88);
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(24) << "Name"
              << std::setw(28) << "Email"
              << std::setw(20) << "Course"
              << std::setw(10) << "Issued"
              << '\n';
    printDivider(88);
}

void Library::printDivider(int width) const {
    std::cout << std::string(width, '-') << '\n';
}
