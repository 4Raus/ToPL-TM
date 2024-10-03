#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class FileHandler {
private:
    const std::vector<std::string> extensions = { ".txt", ".cpp", ".h", ".c" };

public:
    bool CanReadFile(const fs::path& filepath);
    std::string ReadFile(const fs::path& filepath);
    void SaveFile(fs::path& originalFilePath, const std::string& content);
    fs::path FindFileGlobally(const std::string& filename, const std::string& driveLetter, int current_depth, int max_depth);
    fs::path FindFileInDirectories(const std::string& fileName, const std::vector<std::string>& searchPaths);
};

#endif