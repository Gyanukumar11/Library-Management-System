#pragma once
#include <string>
#include <vector>
#include <fstream>

// ============================================================
//  Student – represents a registered library member.
//  Tracks which book IDs have been issued to this student.
// ============================================================
class Student {
public:
    Student() = default;
    Student(int id, const std::string& name, const std::string& email,
            const std::string& course);

    // Accessors
    int                      getId()          const;
    std::string              getName()        const;
    std::string              getEmail()       const;
    std::string              getCourse()      const;
    const std::vector<int>&  getIssuedBooks() const;

    // Mutators
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setCourse(const std::string& course);

    // Issue / return tracking
    bool addIssuedBook(int bookId);      // false if already has it
    bool removeIssuedBook(int bookId);   // false if not found
    bool hasBook(int bookId)       const;
    int  issuedCount()             const;

    // File serialisation
    void serialise(std::ofstream& out)   const;
    void deserialise(std::ifstream& in);

    // Display
    void display()             const;
    void displayIssuedBooks()  const;

private:
    int                m_id     {0};
    std::string        m_name;
    std::string        m_email;
    std::string        m_course;
    std::vector<int>   m_issuedBooks;   // book IDs currently held
};
