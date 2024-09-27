#include <iostream>
#include <string>
#include <fstream>
#include <locale>

using namespace std;

enum CommentType { MULTILINE_COMMENT, SINGLELINE_COMMENT, NO_COMMENT, QUOTE };

class DeletingCommentare {
public:
    string DeleteCom(string& str);
    void UpdateCode(const string& str);

private:
    CommentType CheckCommentType(const char* currentChar);
};

CommentType DeletingCommentare::CheckCommentType(const char* currentChar) {
	if (currentChar[0] == '/' && currentChar[1] == '*') return MULTILINE_COMMENT;
	else if (currentChar[0] == '/' && currentChar[1] == '/') return SINGLELINE_COMMENT;
	else if (currentChar[0] == '"' || currentChar[0] == '\'') return QUOTE;
	return NO_COMMENT;
}

string DeletingCommentare::DeleteCom(string& str) {
	const char* p = str.c_str();
	size_t lenght = str.length();
	size_t i = 0;
	bool inQuote = false;
	char quoteChar = '\0';
	while (i < lenght) {
		if (inQuote) {
			if (p[i] == quoteChar) {
				inQuote = false;
			}
			i++;
			continue;
		}
		CommentType type = CheckCommentType(p + i);
		if (type == QUOTE) {
			inQuote = true;
			quoteChar = p[i];
			i++;
		}
		else if (type == MULTILINE_COMMENT) {
			int start = i;
			i += 2;
			while (i + 1 < lenght && !(p[i] == '*' && p[i + 1] == '/')) i++;
			i += 2;
			str.erase(start, i - start);
			str.insert(start, 1, ' ');
			lenght = str.length();
			p = str.c_str();
			i = start - 1;
		}
		else if (type == SINGLELINE_COMMENT) {
			int start = i;
			i += 2;
			while (i < lenght && p[i] != '\n') i++;
			str.erase(start, i - start);
			lenght = str.length();
			p = str.c_str();
			i = start - 1;
		}
		else i++;
	}
	return str;
}

void DeletingCommentare::UpdateCode(const string& str) {
	ofstream file("C:\\Users\\ASUS\\Desktop\\UNI\\СурГу\\ТЯПиМТ\\tests\\01new_t.txt");
	file << str;
	file.close();
}

int main() {
	setlocale(LC_ALL, "RU");
	DeletingCommentare x;
	string str;
	ifstream file("C:\\Users\\ASUS\\Desktop\\UNI\\СурГу\\ТЯПиМТ\\tests\\01_t.txt");
	getline(file, str, '\0');
	file.close();
	str = x.DeleteCom(str);
	cout << str;
	x.UpdateCode(str);
}