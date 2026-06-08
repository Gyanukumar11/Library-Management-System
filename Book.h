#pragma once
#include <string>
#include <fstream>

// ============================================================
//  Book – represents a single book title in the library.
//  Encapsulates all book-related attributes and file I/O.
// ============================================================
class Book {
public:
    // Construction
    Book() = default;
    Book(int id, const std::string& title, const std::string& author,
         const std::string& genre, int totalCopies);

    // Accessors
    int         getId()            const;
    std::string getTitle()         const;
    std::string getAuthor()        const;
    std::string getGenre()         const;
    int         getTotalCopies()   const;
    int         getAvailableCopies() const;

    // Mutators
    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setGenre(const std::string& genre);
    void setTotalCopies(int n);
    void setAvailableCopies(int n);

    // Operations
    bool issueBook();    // returns false if no copy available
    bool returnBook();   // returns false if all copies already in

    // File serialisation
    void serialise(std::ofstream& out)   const;
    void deserialise(std::ifstream& in);

    // Display
    void display() const;

private:
    int         m_id            {0};
    std::string m_title;
    std::string m_author;
    std::string m_genre;
    int         m_totalCopies   {0};
    int         m_availableCopies {0};
};
