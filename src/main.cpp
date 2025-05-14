// File Utama Program Sistem Kereta Api

// Cara menjalankan pakai g++:
// g++ -std=c++17 -Iinclude src/*.cpp src/data_structure/*.cpp src/management/*.cpp -o SISTEMKAI
// ./SISTEMKAI

#include <management.hpp>
#include <file_path.hpp>
#include <data.hpp>
#include <menu.hpp>
#include <admin.hpp>
#include <user.hpp>

#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>

using namespace std;

int main() {
    ManagementSystem system;
    system.muatDariFile();
    
    int mainChoice;
    do {
        menu_awal();
        cin >> mainChoice;
        
        switch(mainChoice) {
            case 1: {
                string username, password;
                cout << "Username Admin: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;
                
                if (system.authenticateAdmin(username, password)) {
                    menuAdmin(system);
                } else {
                    cout << "Autentikasi gagal!\n";
                }
                break;
            }
            case 2:
                menuUser(system);
                break;
        }
    } while(mainChoice != 3);
    
    system.simpanKeFile();
    return 0;
}