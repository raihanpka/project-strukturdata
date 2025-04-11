#include <menu.hpp>
#include <data.hpp>
#include <management.hpp>
#include <file_path.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>

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
                << "2. Pesan Tiket\n"
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
            case 2: {
                Pemesanan p;
                cout << "Nama Penumpang: ";
                cin.ignore();
                getline(cin, p.namaPenumpang);
                cout << "Nomor Kursi: ";
                getline(cin, p.nomorKursi);
                cout << "Kode Kereta: ";
                getline(cin, p.kodeKereta);
                p.pnr = sys.generatePNR();
                
                try {
                    sys.pesanTiket(p);
                    sys.prosesKonfirmasi();
                } catch(const exception& e) {
                    cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            default:
                cout << "Pilihan tidak valid!\n";
                break;
        }
    } while(choice != 3);
}