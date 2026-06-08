#include "Student.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <sstream>

// ── Construction ─────────────────────────────────────────────
Student::Student(int id, const std::string& name, const std::string& email,
                 const std::string& course)
    : m_id(id), m_name(name), m_email(email), m_course(course)
{
    if (id <= 0)         throw std::invalid_argument("Student ID must be positive.");
    if (name.empty())    throw std::invalid_argument("Name cannot be empty.");
    if (email.empty())   throw std::invalid_argument("Email cannot be empty.");
    if (course.empty())  throw std::invalid_argument("Course cannot be empty.");
}

// ── Accessors ─────────────────────────────────────────────────
int         Student::getId()     const { return m_id; }
std::string Student::getName()   const { return m_name; }
std::string Student::getEmail()  const { return m_email; }
std::string Student::getCourse() const { return m_course; }
const std::vector<int>& Student::getIssuedBooks() const { return m_issuedBooks; }

// ── Mutators ──────────────────────────────────────────────────
void Student::setName(const std::string& n)   { m_name   = n; }
void Student::setEmail(const std::string& e)  { m_email  = e; }
void Student::setCourse(const std::string& c) { m_course = c; }

// ── Issue / Return Tracking ───────────────────────────────────
bool Student::addIssuedBook(int bookId) {
    if (hasBook(bookId)) return false;
    m_issuedBooks.push_back(bookId);
    return true;
}

bool Student::removeIssuedBook(int bookId) {
    auto it = std::find(m_issuedBooks.begin(), m_issuedBooks.end(), bookId);
    if (it == m_issuedBooks.end()) return false;
    m_issuedBooks.erase(it);
    return true;
}

bool Student::hasBook(int bookId) const {
    return std::find(m_issuedBooks.begin(), m_issuedBooks.end(), bookId)
           != m_issuedBooks.end();
}

int Student::issuedCount() const {
    return static_cast<int>(m_issuedBooks.size());
}

// ── File Serialisation ────────────────────────────────────────
// Format: id|name|email|course|bookId1,bookId2,...\n
void Student::serialise(std::ofstream& out) const {
    out << m_id << '|'
        << m_name << '|'
        << m_email << '|'
        << m_course << '|';

    for (size_t i = 0; i < m_issuedBooks.size(); ++i) {
        if (i) out << ',';
        out << m_issuedBooks[i];
    }
    out << '\n';
}

void Student::deserialise(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line)) return;

    auto next = [&](size_t& pos) -> std::string {
        size_t end = line.find('|', pos);
        std::string tok = (end == std::string::npos)
                          ? line.substr(pos)
                          : line.substr(pos, end - pos);
        pos = (end == std::string::npos) ? line.size() : end + 1;
        return tok;
    };

    size_t pos = 0;
    m_id     = std::stoi(next(pos));
    m_name   = next(pos);
    m_email  = next(pos);
    m_course = next(pos);

    std::string booksStr = next(pos);
    m_issuedBooks.clear();
    if (!booksStr.empty()) {
        std::istringstream ss(booksStr);
        std::string tok;
        while (std::getline(ss, tok, ',')) {
            if (!tok.empty()) m_issuedBooks.push_back(std::stoi(tok));
        }
    }
}

// ── Display ───────────────────────────────────────────────────
void Student::display() const {
    std::cout << std::left
              << std::setw(6)  << m_id
              << std::setw(24) << m_name
              << std::setw(28) << m_email
              << std::setw(20) << m_course
              << std::setw(10) << m_issuedBooks.size()
              << '\n';
}

void Student::displayIssuedBooks() const {
    std::cout << "  Student: " << m_name << " (ID: " << m_id << ")\n";
    if (m_issuedBooks.empty()) {
        std::cout << "  No books currently issued.\n";
        return;
    }
    std::cout << "  Issued Book IDs: ";
    for (size_t i = 0; i < m_issuedBooks.size(); ++i) {
        if (i) std::cout << ", ";
        std::cout << m_issuedBooks[i];
    }
    std::cout << '\n';
}
