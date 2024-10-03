#include "CommentRecover.h"
#include "FileHandler.h"
#include "Utils.h"

using namespace std;

void CommentRecover::ProcessDeletingComments(const std::string& filename, const std::string& driveLetter) {
    FileHandler fileHandler;
    Utils::GrantAccessToDrive(driveLetter);
    fs::path foundFilePath = fileHandler.FindFileGlobally(filename, driveLetter, 0, 5);

    if (foundFilePath.empty()) {
        cerr << "Error: file " << filename << " not found globally." << endl;
        return;
    }

    string originalContent = fileHandler.ReadFile(foundFilePath);
    if (originalContent.empty()) return;

    string processedContent;
    singleLineHandler.ProcessComments(originalContent, processedContent);
    multiLineHandler.ProcessComments(originalContent, processedContent);

    fileHandler.SaveFile(foundFilePath, processedContent);
}

void CommentRecover::ProcessDeletingCommentSimple(const std::string& filename, const std::vector<std::string>& searchPaths) {
    FileHandler fileHandler;
    string originalContent, processedContent;
    fs::path foundFilePath = fileHandler.FindFileInDirectories(filename, searchPaths);

    if (foundFilePath.empty()) {
        cerr << "Error: File " << filename << " not found in any of the search paths." << endl;
        return;
    }

    originalContent = fileHandler.ReadFile(foundFilePath);
    if (originalContent.empty()) return;

    singleLineHandler.ProcessComments(originalContent, processedContent);
    multiLineHandler.ProcessComments(originalContent, processedContent);

    fileHandler.SaveFile(foundFilePath, processedContent);
}