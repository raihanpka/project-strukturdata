#include <menu.hpp>
#include <data.hpp>
#include <management.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <cctype> 

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

        // Validasi apakah input adalah angka
        if (cin.fail()) {
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the invalid input
            cout << "Input tidak valid. Silakan masukkan angka.\n";
            cout << "Tekan ENTER untuk melanjutkan...";
            cin.get();
            continue;
        }
        
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
                // Validasi tanggal dengan loop agar tidak lempar exception
                while (true) {
                    cout << "Tanggal (DD-MM-YYYY): ";
                    getline(cin, j.tanggal);
                    if (sys.getJadwalManager().isValidTanggal(j.tanggal)) {
                        break;
                    } else {
                        cout << "Format tanggal tidak valid. Harus DD-MM-YYYY.\n";
                    }
                }
                // Validasi waktu dengan loop agar tidak lempar exception
                while (true) {
                    cout << "Waktu Berangkat (JJ:MM): ";
                    getline(cin, j.waktuBerangkat);
                    cout << "Waktu Tiba (JJ:MM): ";
                    getline(cin, j.waktuTiba);
                    if (sys.getJadwalManager().isValidWaktu(j.waktuBerangkat)) {
                        break;
                    } else if (sys.getJadwalManager().isValidWaktu(j.waktuTiba)) {
                        break;
                    } else {
                        cout << "Format waktu tidak valid. Harus JJ:MM.\n";
                    }
                }
                // Menggenerate kode jadwal
                j.kode = sys.getJadwalManager().generateKodeJadwal(j.namaKereta, j.stasiunAsal, j.stasiunTujuan, j.tanggal);
                cout << "\nKode Kereta yang digenerate: " << j.kode << "\n";
                // Memproses pemesanan tiket
                cout << "\nMemproses pemesanan tiket...\n";
                try {
                    sys.getJadwalManager().tambahJadwal(j);
                    sys.getJadwalManager().prosesKonfirmasiJadwal();
                } catch (const exception& e) {
                    cout << "Terjadi error: " << e.what() << "\n";
                }
                cout << "Tekan ENTER untuk melanjutkan...";
                cin.get();
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
                    sys.getTiketManager().prosesKonfirmasiPemesanan();
                } catch(const exception& e) {
                    cout << "Error: " << e.what() << "\n";
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
            case 4 : {
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