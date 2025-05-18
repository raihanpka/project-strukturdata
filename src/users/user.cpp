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
        
        switch(choice) {
            case 1: {
                string tanggal;
                do {
                    cout << "Masukkan tanggal (DD-MM-YYYY): ";
                    cin.ignore();
                    getline(cin, tanggal);

                    // Memvalidasi format tanggal (DD-MM-YYYY)
                    tm tm = {};
                    istringstream ss(tanggal);
                    ss >> get_time(&tm, "%d-%m-%Y"); // Perbaiki format: %m (bukan %mm)
                    if (ss.fail()) {
                        cout << "Format tanggal tidak valid. Gunakan format DD-MM-YYYY\n";
                        cout << "Tekan ENTER untuk melanjutkan...";
                        cin.clear();
                        cin.get();
                        continue;
                    }
                    break;
                } while(true);
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
            default: {
                cout << "Pilihan tidak valid!\n";
                break;
            }
        }
    } while(choice != 3);
}