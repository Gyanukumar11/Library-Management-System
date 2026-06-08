#include "Book.h"
#include <iostream>
#include <iomanip>
#include <stdexcept>

// ── Construction ────────────────────────────────────────────
Book::Book(int id, const std::string& title, const std::string& author,
           const std::string& genre, int totalCopies)
    : m_id(id), m_title(title), m_author(author),
      m_genre(genre), m_totalCopies(totalCopies),
      m_availableCopies(totalCopies)
{
    if (id <= 0)          throw std::invalid_argument("Book ID must be positive.");
    if (title.empty())    throw std::invalid_argument("Title cannot be empty.");
    if (author.empty())   throw std::invalid_argument("Author cannot be empty.");
    if (totalCopies < 1)  throw std::invalid_argument("Total copies must be >= 1.");
}

// ── Accessors ───────────────────────────────────────────────
int         Book::getId()              const { return m_id; }
std::string Book::getTitle()           const { return m_title; }
std::string Book::getAuthor()          const { return m_author; }
std::string Book::getGenre()           const { return m_genre; }
int         Book::getTotalCopies()     const { return m_totalCopies; }
int         Book::getAvailableCopies() const { return m_availableCopies; }

// ── Mutators ─────────────────────────────────────────────────
void Book::setTitle(const std::string& title)   { m_title  = title; }
void Book::setAuthor(const std::string& author) { m_author = author; }
void Book::setGenre(const std::string& genre)   { m_genre  = genre; }
void Book::setTotalCopies(int n)                { m_totalCopies = n; }
void Book::setAvailableCopies(int n)            { m_availableCopies = n; }

// ── Operations ───────────────────────────────────────────────
bool Book::issueBook() {
    if (m_availableCopies <= 0) return false;
    --m_availableCopies;
    return true;
}

bool Book::returnBook() {
    if (m_availableCopies >= m_totalCopies) return false;
    ++m_availableCopies;
    return true;
}

// ── File Serialisation ───────────────────────────────────────
// Format (binary-text hybrid – pipe-delimited lines):
//   id|title|author|genre|totalCopies|availableCopies\n
void Book::serialise(std::ofstream& out) const {
    out << m_id << '|'
        << m_title << '|'
        << m_author << '|'
        << m_genre << '|'
        << m_totalCopies << '|'
        << m_availableCopies << '\n';
}

void Book::deserialise(std::ifstream& in) {
    std::string line;
    if (!std::getline(in, line)) return;

    // Parse pipe-separated tokens
    auto next = [&](size_t& pos) -> std::string {
        size_t end = line.find('|', pos);
        std::string tok = (end == std::string::npos)
                          ? line.substr(pos)
                          : line.substr(pos, end - pos);
        pos = (end == std::string::npos) ? line.size() : end + 1;
        return tok;
    };

    size_t pos = 0;
    m_id              = std::stoi(next(pos));
    m_title           = next(pos);
    m_author          = next(pos);
    m_genre           = next(pos);
    m_totalCopies     = std::stoi(next(pos));
    m_availableCopies = std::stoi(next(pos));
}

// ── Display ──────────────────────────────────────────────────
void Book::display() const {
    std::cout << std::left
              << std::setw(6)  << m_id
              << std::setw(32) << m_title
              << std::setw(22) << m_author
              << std::setw(16) << m_genre
              << std::setw(8)  << m_totalCopies
              << std::setw(10) << m_availableCopies
              << '\n';
}
