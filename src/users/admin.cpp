#include <menu.hpp>
#include <data.hpp>
#include <management.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

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
                << "2. Pesan Tiket Penumpang\n"
                << "3. Lihat Semua Jadwal\n"
                << "4. Kembali\n"
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
                
                j.kode = sys.getJadwalManager().generateKodeJadwal(j.namaKereta, j.stasiunAsal, j.stasiunTujuan, j.tanggal);
                cout << "Kode Kereta yang digenerate: " << j.kode << "\n";
                cout << "Jadwal berhasil ditambahkan!\n";
                cout << "Tekan ENTER untuk melanjutkan...";
                cin.get();
                sys.getJadwalManager().tambahJadwal(j);
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
                p.pnr = sys.getTiketManager().generatePNR();
                cout << "PNR yang digenerate: " << p.pnr << "\n";
                cout << "Apakah Anda yakin ingin memesan tiket? (y/n): ";
                char confirm;
                cin >> confirm;
                if (confirm != 'y' && confirm != 'Y') {
                    cout << "Pesanan dibatalkan.\n";
                    break;
                }
                // Memvalidasi ketersediaan kursi
                if (!sys.getTiketManager().isSeatAvailable(p.kodeKereta, p.nomorKursi)) {
                    cout << "Kursi tidak tersedia!\n";
                    break;
                }
                // Memproses pemesanan tiket
                cout << "Memproses pemesanan tiket...\n";
                cout << "Tekan ENTER untuk melanjutkan...";
                cin.get();
                try {
                    sys.getTiketManager().pesanTiket(p);
                    sys.getTiketManager().prosesKonfirmasi();
                } catch(const exception& e) {
                    cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 3: {
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
                sys.getJadwalManager().tampilkanJadwal(tanggal);
                break;
            }
            default: {
                cout << "Pilihan tidak valid!\n";
                break;
            }
        }
    } while(choice != 4);
}