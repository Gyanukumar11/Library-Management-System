#pragma once
#include "Book.h"
#include "Student.h"
#include <map>
#include <string>
#include <vector>
#include <optional>

// ============================================================
//  Library – central controller / facade.
//  Owns all Books and Students; mediates every operation.
//  Uses std::map<int, Book> and std::map<int, Student> for
//  O(log n) lookup by ID.
// ============================================================
class Library {
public:
    explicit Library(const std::string& bookFile    = "books.dat",
                     const std::string& studentFile = "students.dat");
    ~Library();   // auto-save on destruction

    // ── Book Management ──────────────────────────────────────
    bool addBook(int id, const std::string& title, const std::string& author,
                 const std::string& genre, int copies);
    bool deleteBook(int id);

    // Search returns nullptr / empty if not found
    Book*                   findBookById(int id);
    const Book*             findBookById(int id) const;
    std::vector<const Book*> findBooksByTitle(const std::string& query) const;

    void displayAllBooks()      const;
    void displayAvailableBooks() const;

    // ── Student Management ───────────────────────────────────
    bool registerStudent(int id, const std::string& name,
                         const std::string& email, const std::string& course);
    Student*       findStudentById(int id);
    const Student* findStudentById(int id) const;

    void displayAllStudents() const;

    // ── Issue / Return ───────────────────────────────────────
    bool issueBook(int studentId, int bookId);
    bool returnBook(int studentId, int bookId);

    void displayIssuedBooks() const;   // list all currently-issued records

    // ── Statistics ───────────────────────────────────────────
    void displayStatistics() const;

    // ── Persistence ─────────────────────────────────────────
    void saveAll()   const;
    void loadAll();

private:
    std::map<int, Book>    m_books;
    std::map<int, Student> m_students;

    std::string m_bookFile;
    std::string m_studentFile;

    // Helpers
    void printBookHeader()    const;
    void printStudentHeader() const;
    void printDivider(int width) const;
};
