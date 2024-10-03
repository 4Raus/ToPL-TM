#ifndef UTILS_H
#define UTILS_H

#include <string>

class Utils {
public:
    static void ClearMemory(std::string& content, size_t memory_limit);
    static void GrantAccessToDrive(const std::string& driveLetter);
};

#endif