#pragma once
#include <string>
#include <limits>
#include <iostream>

// ============================================================
//  Utils – stateless helper functions for UI and validation.
//  Kept in a separate translation unit to keep main.cpp clean.
// ============================================================
namespace Utils {

    // Read an integer from stdin; re-prompt on bad input
    inline int readInt(const std::string& prompt, int minVal = 1,
                       int maxVal = std::numeric_limits<int>::max())
    {
        int value{};
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value >= minVal && value <= maxVal) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Invalid input. Enter a number";
            if (maxVal != std::numeric_limits<int>::max())
                std::cout << " between " << minVal << " and " << maxVal;
            std::cout << ".\n";
        }
    }

    // Read a non-empty trimmed string
    inline std::string readString(const std::string& prompt,
                                  size_t maxLen = 128)
    {
        std::string s;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, s);

            // Trim leading/trailing spaces
            size_t start = s.find_first_not_of(" \t");
            size_t end   = s.find_last_not_of(" \t");
            if (start != std::string::npos) s = s.substr(start, end - start + 1);
            else s.clear();

            if (!s.empty() && s.size() <= maxLen) return s;
            std::cout << "  [!] Input cannot be empty";
            if (s.size() > maxLen)
                std::cout << " and must be <= " << maxLen << " characters";
            std::cout << ".\n";
        }
    }

    // Basic email format check (contains @ and .)
    inline bool isValidEmail(const std::string& email) {
        auto at = email.find('@');
        if (at == std::string::npos || at == 0) return false;
        auto dot = email.find('.', at);
        return (dot != std::string::npos && dot < email.size() - 1);
    }

    // Clear the console in a cross-platform way
    inline void clearScreen() {
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
    }

    // Pause until Enter is pressed
    inline void pause() {
        std::cout << "\n  Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Thin banner printer
    inline void printBanner() {
        std::cout << R"(
 ╔══════════════════════════════════════════════════╗
 ║          LIBRARY MANAGEMENT SYSTEM               ║
 ║              C++17 Edition                       ║
 ╚══════════════════════════════════════════════════╝
)";
    }

} // namespace Utils
