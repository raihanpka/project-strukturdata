#include "user_role.hpp"
#include <fstream>
#include <stdexcept>

using namespace std;

bool UserContext::authenticateAdmin(const string& uname, const string& pass) {
    ifstream file("data/admin_credentials.txt");
    string line;
    while (getline(file, line)) {
        size_t delimiter = line.find(':');
        if (delimiter != string::npos) {
            string fileUsername = line.substr(0, delimiter);
            string filePassword = line.substr(delimiter + 1);
            if (uname == fileUsername && pass == filePassword) {
                role = UserRole::ADMIN;
                username = uname;
                return true;
            }
        }
    }
    return false;
}

void UserContext::setPassenger() {
    role = UserRole::PASSENGER;
    username = "Guest";
}

UserRole UserContext::getRole() const {
    return role;
}

string UserContext::getUsername() const {
    return username;
}
