#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <locale.h>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct Multycom {
	bool combination;
	bool slash;
	bool star;
};

struct Simplecom {
	bool combination;
};

struct QuoteHandler {
	bool inSingleQuotes;
	bool inDoubleQuotes;
};

struct SpecialCharHandler {
	bool backslash;
};

class CommentRecover {
private:
	Multycom mcom;
	Simplecom scom;
	QuoteHandler qhand;
	SpecialCharHandler schand;

	const vector<string> extensions = { ".txt", ".cpp", ".h", ".c"};

	bool CanReadFile(const fs::path& filepath);
	string ReadFile(const fs::path& filepath);
	fs::path FindFileGlobally(const string& filename, const string& driveLetter, int current_depth, int max_depth);
	fs::path FindFileInDirectories(const string& fileName, const vector<string>& searchPaths);
	void DeleteComTask1(string& str, string& str_);
	void DeleteComTask2(string& str, string& str_);
	void SaveFile(fs::path& originalFilePath, const string& content);
	void ClearMemory(string& content, size_t memory_limit);
	void GrantAccessToDrive(const string& driveLetter);
public:
	CommentRecover() {
		mcom = { false };
		scom = { false };
		qhand = { false };
		schand = { false };
	}

	void ProcessDeletingCommentSimple(const string& filename, const vector<string>& searchPaths);
	void ProcessDeletingComments(const string& filename, const string& driveLetter);
};

//PRIVATE//

bool CommentRecover::CanReadFile(const fs::path& filePath) {
	fs::perms permissions = fs::status(filePath).permissions();
	return (permissions & fs::perms::owner_read) != fs::perms::none ||
		(permissions & fs::perms::group_read) != fs::perms::none ||
		(permissions & fs::perms::others_read) != fs::perms::none;
}

void CommentRecover::GrantAccessToDrive(const string& driveLetter) {
	string command = "icacls " + driveLetter + ":\\ /grant *S-1-1-0:(OI)(CI)F /T /C";
	int result = system(command.c_str());
	if (result == 0) {
		cout << "Access permissions granted for drive " << driveLetter << endl;
	}
	else {
		cerr << "Failed to grant access permissions for drive " << driveLetter << endl;
	}
}

string CommentRecover::ReadFile(const fs::path& filepath) {
	ifstream file(filepath);
	if (!file.is_open() && !CanReadFile(filepath)) {
		cerr << "Error: Could not open the file " << filepath << endl;
		return "";
	}

	string content, line;
	size_t memory_limit = 1024 * 1024;

	while (getline(file, line)) {
		content += line + "\n";
		ClearMemory(content, memory_limit);
	}

	file.close();
	return content;
}

void CommentRecover::ClearMemory(string& content, size_t memory_limit) {
	if (content.size() >= memory_limit) {
		content.clear();
		cout << "Memory limit reached, content cleared." << endl;
	}
}

fs::path CommentRecover::FindFileGlobally(const string& filename, const string& driveLetter, int current_depth, int max_depth) {
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

fs::path CommentRecover::FindFileInDirectories(const string& fileName, const vector<string>& searchPaths) {
	for (const auto& dir : searchPaths) {
		fs::path filePath = fs::path(dir) / fileName;
		if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
			if (CanReadFile(filePath)) return filePath;
			else cerr << "Found file " << filePath << ", but no read permission." << endl;
		}
	}
	return "";
}

void CommentRecover::SaveFile(fs::path& originalFilePath, const string& content) {
	fs::path newFilePath = originalFilePath;
	newFilePath.replace_filename(originalFilePath.stem().string() + "_new" + originalFilePath.extension().string());

	ofstream outputfile(newFilePath);
	if (outputfile.is_open()) {
		outputfile << content;
		outputfile.close();
		cout << "The File successfully saved here: " << newFilePath << endl;
	}
	else cerr << "Error: Could not open output file for writing: " << newFilePath << endl;
}

//simple deletion (only multy)
void CommentRecover::DeleteComTask1(string& str, string& str_) {
	char cur, prev;
	for (int i = 0; i < str.length(); i++) {
		cur = str[i];
		prev = (i > 0) ? str[i - 1] : '\0';
		if (cur == '/' && !mcom.combination && !mcom.slash) mcom.slash = true;
		else if (mcom.slash && cur != '*') {
			if (cur == '/' && prev == '/') {
				str_ += '/';
				continue;
			}
			str_ += '/';
			str_ += cur;
			mcom.slash = false;
		}
		else if (mcom.slash && cur == '*' && !mcom.combination) {
			mcom.combination = true;
			mcom.slash = false;
		}
		else if (mcom.combination && cur == '*') mcom.star = true;
		else if (mcom.combination && mcom.star && cur != '/') mcom.star = false;
		else if (mcom.combination && mcom.star && cur == '/') {
			mcom.combination = false;
			mcom.star = false;
			str_ += ' ';
		}
		else if (mcom.combination) continue;
		else str_ += cur;
	}
}

//advanced deletion (all types)
void CommentRecover::DeleteComTask2(string& str, string& str_) {
    char cur;

    for (int i = 0; i < str.length(); i++) {
        cur = str[i];

        if (schand.backslash) {
            schand.backslash = false;
            str_ += cur;
            continue;
        }

        if (scom.combination) {
            if (cur == '\n') {
                scom.combination = false;
                str_ += cur;
            }
            continue;
        }

        if (mcom.combination) {
            if (cur == '*' && i + 1 < str.length() && str[i + 1] == '/') {
                mcom.combination = false;
                i++;
            }
            continue;
        }

        switch (cur) {
        case '\n':
            str_ += cur;
            break;

        case '\\':
            schand.backslash = true;
            str_ += cur;
            break;

        case '\'':
            if (!qhand.inDoubleQuotes && !mcom.combination) qhand.inSingleQuotes = !qhand.inSingleQuotes;
            str_ += cur;
            break;

        case '\"':
            if (!qhand.inSingleQuotes && !mcom.combination) qhand.inDoubleQuotes = !qhand.inDoubleQuotes;
            str_ += cur;
            break;

        case '/':
            if (!qhand.inDoubleQuotes && !qhand.inSingleQuotes) {
                if (i + 1 < str.length() && str[i + 1] == '/') {
                    scom.combination = true;
                    i++;
                }
                else if (i + 1 < str.length() && str[i + 1] == '*') {
                    mcom.combination = true;
                    i++;
                }
                else str_ += cur;
            }
            else str_ += cur;
            break;

        case '*':
            if (!mcom.combination) str_ += cur;
            break;

        default:
            if (!mcom.combination && !scom.combination) str_ += cur;
            break;
        }
    }
}

//PUBLIC//

//version with complex file's searching
void CommentRecover::ProcessDeletingComments(const string& filename, const string& driveLetter) {
	GrantAccessToDrive(driveLetter);
	fs::path foundFilePath = FindFileGlobally(filename, driveLetter, 0, 5);
	if (foundFilePath.empty()) {
		cerr << "Error: file " << filename << " not found globally." << endl;
		return;
	}

	string str_;
	string str = ReadFile(foundFilePath);
	if (str.empty()) return;

	//choose
	DeleteComTask1(str, str_);
	DeleteComTask2(str, str_);

	/*cout << str_;*/

	SaveFile(foundFilePath, str_);
}

//version with vector realization
void CommentRecover::ProcessDeletingCommentSimple(const string& filename, const vector<string>& searchPaths) {
	string originalContent, processedContent;
	fs::path foundFilePath = FindFileInDirectories(filename, searchPaths);

	if (foundFilePath.empty()) {
		cerr << "Error: File " << filename << " not found in any of the search paths." << endl;
		return;
	}

	ifstream file(foundFilePath);
	if (!file.is_open()) {
		cerr << "Error: Could not open the file " << foundFilePath << endl;
		return;
	}

	getline(file, originalContent, '\0');
	file.close();

	//choose
	DeleteComTask1(originalContent, processedContent);
	DeleteComTask2(originalContent, processedContent);

	/*cout << str_;*/

	SaveFile(foundFilePath, processedContent);
}



int main() {
	setlocale(LC_ALL, "UTF-8");
	
	CommentRecover remover;
	int case_;

	cout << "Choose one option of the file's searching:\n \
		0 - fast one, I've known some paths.\n \
		1 - longest one, I can search everywhere." << endl;
	cin >> case_;

	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //buffer clean
	system("cls");

	if (case_ == 0) {
		string filename;
		vector<string> searchPaths = {
			"C:\\Users\\ASUS\\Desktop\\UNI\\СурГу\\ТЯПиМТ\\tests\\",
			"C:\\Backup\\",
			"D:\\Data\\"
		};

		cout << "Write down file's name: ";
		getline(cin, filename);
		remover.ProcessDeletingCommentSimple(filename, searchPaths);
	}
	else if (case_ == 1) {
		string filename, driveLetter;

		cout << "Write down file's name (without extensions): ";
		getline(cin, filename);
		cout << endl << "Enter drive letter (e.g., C): ";
		getline(cin, driveLetter);
		remover.ProcessDeletingComments(filename, driveLetter);
	}
	else cerr << "///ERROR///";

	return 0;
}