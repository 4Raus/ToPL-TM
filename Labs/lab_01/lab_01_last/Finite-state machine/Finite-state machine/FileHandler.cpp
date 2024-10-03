#include "FileHandler.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

using namespace std;

bool FileHandler::CanReadFile(const fs::path& filePath) {
    fs::perms permissions = fs::status(filePath).permissions();
    return (permissions & fs::perms::owner_read) != fs::perms::none ||
        (permissions & fs::perms::group_read) != fs::perms::none ||
        (permissions & fs::perms::others_read) != fs::perms::none;
}

std::string FileHandler::ReadFile(const fs::path& filepath) {
    ifstream file(filepath);
    if (!file.is_open() && !CanReadFile(filepath)) {
        cerr << "Error: Could not open the file " << filepath << endl;
        return "";
    }

    string content, line;
    size_t memory_limit = 1024 * 1024;

    while (getline(file, line)) {
        content += line + "\n";
        Utils::ClearMemory(content, memory_limit);
    }

    file.close();
    return content;
}

void FileHandler::SaveFile(fs::path& originalFilePath, const std::string& content) {
    fs::path newFilePath = originalFilePath;
    newFilePath.replace_filename(originalFilePath.stem().string() + "_new" + originalFilePath.extension().string());

    ofstream outputfile(newFilePath);
    if (outputfile.is_open()) {
        outputfile << content;
        outputfile.close();
        cout << "The File successfully saved here: " << newFilePath << endl;
    }
    else {
        cerr << "Error: Could not open output file for writing: " << newFilePath << endl;
    }
}

fs::path FileHandler::FindFileGlobally(const std::string& filename, const std::string& driveLetter, int current_depth, int max_depth) {
    fs::path drivePath = driveLetter + ":/";

    if (!fs::exists(drivePath) || !fs::is_directory(drivePath)) {
        cerr << "Error: Disk " << driveLetter << " is not accessible or does not exist." << endl;
        return "";
    }

    if (current_depth > max_depth) {
        cerr << "Max depth reached, skipping deeper directories." << endl;
        return "";
    }

    for (const auto& entry : fs::directory_iterator(drivePath, fs::directory_options::skip_permission_denied)) {
        try {
            if (entry.is_directory()) {
                fs::path subdirectory = entry.path();
                fs::path foundPath = FindFileGlobally(filename, subdirectory.string(), current_depth + 1, max_depth);
                if (!foundPath.empty()) return foundPath;
            }
            else if (entry.is_regular_file()) {
                string fileStem = entry.path().stem().string();
                if (fileStem == filename) {
                    string ext = entry.path().extension().string();
                    if (find(extensions.begin(), extensions.end(), ext) != extensions.end())
                        return entry.path();
                }
            }
        }
        catch (const fs::filesystem_error& ex) {
            cerr << "Filesystem error: " << ex.what() << endl;
            continue;
        }
    }
    return "";
}

fs::path FileHandler::FindFileInDirectories(const std::string& fileName, const std::vector<std::string>& searchPaths) {
    for (const auto& dir : searchPaths) {
        fs::path filePath = fs::path(dir) / fileName;
        if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
            if (CanReadFile(filePath)) return filePath;
            else cerr << "Found file " << filePath << ", but no read permission." << endl;
        }
    }
    return "";
}