#include <iostream>

using namespace std;

string DeleteCom(string& str) {
	string str_;
	char cur;
	bool combination = false, slash = false, star = false;
	for (int i = 0; i < str.length(); i++) {
		cur = str[i];
		if (cur == '/') slash = true;
		else if (slash == true && cur == '*') {
			combination = true;
			slash = false;
		}
		else if (combination == true && cur == '*') star = true;
		else if (combination == true && star == true && cur == '/') {
			combination = false;
			star = false;
		}
		else if (combination == true && star == true) continue;
		else str_ += cur;
	}
	return str_;
}

void UpdateCode(const string& str) {
	ofstream file("C:\\Users\\ASUS\\Desktop\\UNI\\ÑóðÃó\\ÒßÏèÌÒ\\tests\\01new_t.txt");
	file << str;
	file.close();
}

int main() {
	setlocale(LC_ALL, "RU");
	string str;
	ifstream file("C:\\Users\\ASUS\\Desktop\\UNI\\ÑóðÃó\\ÒßÏèÌÒ\\tests\\01_t.txt");
	getline(file, str, '\0');
	file.close();
	str = DeleteCom(str);
	cout << str;
	UpdateCode(str);
}