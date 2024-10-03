#include "Utils.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void Utils::ClearMemory(std::string& content, size_t memory_limit) {
    if (content.size() >= memory_limit) {
        content.clear();
        cout << "Memory limit reached, content cleared." << endl;
    }
}

void Utils::GrantAccessToDrive(const std::string& driveLetter) {
    string command = "icacls " + driveLetter + ":\\ /grant *S-1-1-0:(OI)(CI)F /T /C";
    int result = system(command.c_str());
    if (result == 0) {
        cout << "Access permissions granted for drive " << driveLetter << endl;
    }
    else {
        cerr << "Failed to grant access permissions for drive " << driveLetter << endl;
    }
}
