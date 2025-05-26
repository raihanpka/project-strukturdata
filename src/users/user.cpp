#include <menu.hpp>
#include <data.hpp>
#include <management.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

void menuUser(ManagementSystem& sys) {
    int choice;
    do {
        clrscr();
        cout << BLUE << " _____ _____ _____ _____ ________  ___ _   __  ___  _____ " << endl;
        cout << BLUE << "/  ___|_   _/  ___|_   _|  ___|  \\/  || | / / / _ \\|_   _|" << endl;
        cout << BLUE << "\\ `--.  | | \\ `--.  | | | |__ | .  . || |/ / / /_\\ \\ | |  " << endl;
        cout << BLUE << " `--. \\ | |  `--. \\ | | |  __|| |\\/| ||    \\ |  _  | | |  " << endl;
        cout << BLUE << "/\\__/ /_| |_/\\__/ / | | | |___| |  | || |\\  \\| | | |_| |_ " << endl;
        cout << BLUE << "\\____/ \\___/\\____/  \\_/ \\____/\\_|  |_/\\_| \\_/\\_| |_/\\___/ " << endl;
        cout << WHITE 
                << "\nMenu Penumpang:\n"
                << "1. Lihat Jadwal\n"
                << "2. Cari Tiket Penumpang\n"
                << "3. Kembali\n"
                << "Pilih: ";
        cin >> choice;

        // Validasi apakah input adalah angka
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid. Silakan masukkan angka.\n";
            cout << "Tekan ENTER untuk melanjutkan...";
            cin.get();
            continue;
        }
        
        switch(choice) {
            case 1: {
                string tanggal;
                cin.ignore();
                while (true) {
                    cout << "Masukkan Tanggal (DD-MM-YYYY): ";
                    getline(cin, tanggal);
                    if (sys.getJadwalManager().isValidTanggal(tanggal)) {
                        break;
                    } else {
                        cout << "Format tanggal tidak valid. Harus DD-MM-YYYY.\n";
                    }
                }
                sys.getJadwalManager().tampilkanJadwal(tanggal);
                break;
            }
            case 2: {
                string pnr;
                cout << "Masukkan PNR Tiket: ";
                cin.ignore();
                getline(cin, pnr);
                if (pnr.empty()) {
                    cout << "PNR tidak boleh kosong!\n";
                    cout << "Tekan ENTER untuk melanjutkan...";
                    cin.get();
                    continue;
                }
                sys.getTiketManager().tampilkanTiketByPNR(pnr);
                break;
            }
            case 3: {
                cout << "Kembali ke menu utama...\n";
                return;
            }
            default: {
                cout << "Pilihan tidak valid!\n";
                return;
            }
        }
    } while(true);
}