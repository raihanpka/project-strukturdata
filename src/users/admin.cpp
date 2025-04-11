#include <menu.hpp>
#include <data.hpp>
#include <management.hpp>
#include <file_path.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>

void menuAdmin(ManagementSystem& sys) {
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
                << "\nMenu Admin:\n"
                << "1. Tambah Jadwal Kereta\n"
                << "2. Lihat Semua Pemesanan\n"
                << "3. Kembali\n"
                << "Pilih: ";
        cin >> choice;
        
        switch(choice) {
            case 1: {
                Jadwal j;
                cout << "Stasiun Asal: ";
                cin.ignore();
                getline(cin, j.stasiunAsal);
                cout << "Stasiun Tujuan: ";
                getline(cin, j.stasiunTujuan);
                cout << "Nama Kereta: ";
                getline(cin, j.namaKereta);
                cout << "Tanggal (DD-MM-YYYY): ";
                getline(cin, j.tanggal);
                cout << "Waktu Berangkat: ";
                getline(cin, j.waktuBerangkat);
                cout << "Waktu Tiba: ";
                getline(cin, j.waktuTiba);
                
                j.kode = sys.generateKodeJadwal(j.namaKereta, j.stasiunAsal, j.stasiunTujuan, j.tanggal);
                cout << "Kode Kereta yang digenerate: " << j.kode << "\n";
                
                sys.tambahJadwal(j);
                break;
            }
            case 2: {
                string tanggal;
                do {
                    cout << "Masukkan tanggal (DD-MM-YYYY): ";
                    cin.ignore();
                    getline(cin, tanggal);
                    
                // Memvalidasi format tanggal (DD-MM-YYYY)
                    tm tm = {};
                    istringstream ss(tanggal);
                    ss >> get_time(&tm, "%d-%m-%Y");
                    if (ss.fail()) {
                        cout << "Format tanggal tidak valid. Gunakan format DD-MM-YYYY\n";
                        continue;
                    }
                    break;
                } while(true);
                sys.tampilkanJadwal(tanggal);
                break;
            }
            default:
                cout << "Pilihan tidak valid!\n";
                break;
        }
    } while(choice != 3);
}