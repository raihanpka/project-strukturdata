// main.cpp - Entry Point
#include "management.hpp" // controller utama
#include "menu.hpp"
#include "admin.hpp"
#include "user.hpp"
#include <iostream>
#include <limits>

using namespace std;

int main() {
    ManagementSystem system;
    system.muatDariFile();

    int pilihan;
    do {
        menu_awal();
        cin >> pilihan;
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
                break;
            }
        }
    } while (pilihan != 3);

    system.simpanKeFile();
    return 0;
}