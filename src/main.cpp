// main.cpp - Entry Point
#include "management.hpp" // controller utama
#include "menu.hpp"
#include "admin.hpp"
#include "user.hpp"
#include <iostream>
#include <sstream>
#include <limits>
#include <cctype>

using namespace std;

int main() {
    srand(time(0));
    ManagementSystem system;
    system.muatDariFile();

    int pilihan;
    do {
        menu_awal();
        cin >> pilihan;
        
        // Validasi apakah input adalah angka
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
            cout << "Input tidak valid. Silakan masukkan angka.\n";
            cout << "Tekan ENTER untuk melanjutkan...";
            cin.get();
            continue;
        }

        switch (pilihan) {
            case 1: {
                string username, password;
                bool loginSukses = false;
                do {
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Username Admin: ";
                    getline(cin, username);
                    cout << "Password: ";
                    getline(cin, password);
                    if (system.getUser().authenticateAdmin(username, password)) {
                        loginSukses = true;
                        menuAdmin(system);
                    } else {
                        header();
                        cout << "Autentikasi gagal! Coba lagi.\n";
                        cout << "Tekan ENTER untuk melanjutkan...\n";
                    }
                } while (!loginSukses);
                break;
            }
            case 2:{
                system.getUser().setPassenger();
                menuUser(system);
                break;
            }
            case 3: {
                cout << "Keluar dari program...\n";
                cout << BLUE << "\nTerima kasih telah menggunakan SISTEMKAI!\nSampai Jumpa!\n" << WHITE << endl;
                return 0;
            }
        }
    } while (true);

    system.simpanKeFile();
    return 0;
}