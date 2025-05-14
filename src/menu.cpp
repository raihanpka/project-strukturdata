#include <menu.hpp>
#include <admin.hpp>

#include <iostream>
#include <limits>

using namespace std;

void clrscr() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_input() {
    cout << "Tekan ENTER untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void menu_awal() {
    clrscr();
    cout << BLUE << " _____ _____ _____ _____ ________  ___ _   __  ___  _____ " << endl;
    cout << BLUE << "/  ___|_   _/  ___|_   _|  ___|  \\/  || | / / / _ \\|_   _|" << endl;
    cout << BLUE << "\\ `--.  | | \\ `--.  | | | |__ | .  . || |/ / / /_\\ \\ | |  " << endl;
    cout << BLUE << " `--. \\ | |  `--. \\ | | |  __|| |\\/| ||    \\ |  _  | | |  " << endl;
    cout << BLUE << "/\\__/ /_| |_/\\__/ / | | | |___| |  | || |\\  \\| | | |_| |_ " << endl;
    cout << BLUE << "\\____/ \\___/\\____/  \\_/ \\____/\\_|  |_/\\_| \\_/\\_| |_/\\___/ " << endl;
    cout << endl;
    cout << WHITE << "Halo, Selamat Datang di Aplikasi SISTEM KAI" << endl;
    cout << WHITE << "1. Login sebagai Admin" << endl;
    cout << WHITE << "2. Login sebagai Pengguna" << endl;
    cout << WHITE << "3. Exit" << endl;
    cout << WHITE << "Masukkan pilihanmu: ";
}