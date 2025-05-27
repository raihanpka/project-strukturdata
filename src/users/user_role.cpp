#include "user_role.hpp"
#include <fstream>
#include <stdexcept>

using namespace std;

// Fungsi autentikasi admin berdasarkan username dan password
bool UserContext::authenticateAdmin(const string& uname, const string& pass) {
    ifstream file("data/admin_credentials.txt"); // Buka file kredensial admin
    string line;
    while (getline(file, line)) { // Baca setiap baris file
        size_t delimiter = line.find(':'); // Cari posisi pemisah ':'
        if (delimiter != string::npos) { // Jika format benar
            string fileUsername = line.substr(0, delimiter); // Ambil username dari file
            string filePassword = line.substr(delimiter + 1); // Ambil password dari file
            if (uname == fileUsername && pass == filePassword) { // Cek kecocokan
                role = UserRole::ADMIN; // Set role sebagai admin
                username = uname;       // Simpan username
                return true;            // Autentikasi berhasil
            }
        }
    }
    return false; // Autentikasi gagal
}

// Set role sebagai penumpang (guest)
void UserContext::setPassenger() {
    role = UserRole::PASSENGER; // Set role penumpang
    username = "Guest";         // Set username default
}

// Mengambil role user saat ini
UserRole UserContext::getRole() const {
    return role;
}

// Mengambil username user saat ini
string UserContext::getUsername() const {
    return username;
}