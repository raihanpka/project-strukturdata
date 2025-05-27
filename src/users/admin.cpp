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

// Fungsi utama menu untuk admin
void menuAdmin(ManagementSystem& sys) {
    int choice;
    do {
        // Tampilkan judul dan menu utama admin
        header();
        cout << WHITE  
                << "Menu Admin:\n"
                << "1. Kelola Jadwal Kereta\n"
                << "2. Kelola Tiket Penumpang\n"
                << "3. Lihat Jadwal Kereta\n"
                << "4. Kembali\n"
                << "Pilih: ";
        cin >> choice;

        // Validasi input angka
        if (cin.fail()) {
            cin.clear(); // Reset error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang input tidak valid
            cout << "Input tidak valid. Silakan masukkan angka.\n";
            cout << "Tekan ENTER untuk melanjutkan...";
            cin.get();
            continue;
        }
        
        switch(choice) {
            case 1: { // Kelola Jadwal Kereta
                int subChoice;
                do {
                    sys.muatDariFile(); // Muat data terbaru dari file
                    header();
                    cout << "Kelola Jadwal Kereta:\n";
                    cout << "1. Tambah Jadwal\n";
                    cout << "2. Edit Jadwal\n";
                    cout << "3. Hapus Jadwal\n";
                    cout << "4. Kembali\n";
                    cout << "Pilih: ";
                    cin >> subChoice;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Input tidak valid. Silakan masukkan angka.\n";
                        continue;
                    }
                    if (subChoice == 1) { 
                        header();
                        cin.ignore();
                        // Tambah Jadwal
                        Jadwal j;
                        cout << "Stasiun Asal: ";
                        getline(cin, j.stasiunAsal);
                        cout << "Stasiun Tujuan: ";
                        getline(cin, j.stasiunTujuan);
                        cout << "Nama Kereta: ";
                        getline(cin, j.namaKereta);
                        while (true) {
                            cout << "Tanggal (DD-MM-YYYY): ";
                            getline(cin, j.tanggal);
                            if (sys.getJadwalManager().isValidTanggal(j.tanggal)) break;
                            cout << "Format tanggal tidak valid. Harus DD-MM-YYYY.\n";
                        }
                        while (true) {
                            cout << "Waktu Berangkat (JJ:MM): ";
                            getline(cin, j.waktuBerangkat);
                            cout << "Waktu Tiba (JJ:MM): ";
                            getline(cin, j.waktuTiba);
                            if (sys.getJadwalManager().isValidWaktu(j.waktuBerangkat) && sys.getJadwalManager().isValidWaktu(j.waktuTiba)) break;
                            cout << "Format waktu tidak valid. Harus JJ:MM.\n";
                        }
                        j.kode = sys.getJadwalManager().hashingKodeJadwal(j.namaKereta, j.stasiunAsal, j.stasiunTujuan, j.tanggal);
                        cout << "\nKode Kereta yang digenerate: " << j.kode << "\n";
                        try {
                            sys.getJadwalManager().tambahJadwal(j); // Tambah ke daftar
                            sys.getJadwalManager().prosesKonfirmasiJadwal(); // Konfirmasi admin
                        } catch (const exception& e) {
                            cout << "Terjadi error: " << e.what() << "\n";
                        }
                        cout << "Tekan ENTER untuk melanjutkan...";
                        cin.get();
                        sys.simpanKeFile();
                    } else if (subChoice == 2) { 
                        header();
                        cin.ignore();
                        // Edit Jadwal
                        string kode;
                        cout << "Masukkan kode jadwal yang akan diedit: ";
                        getline(cin, kode);
                        Jadwal jBaru;
                        cout << "Stasiun Asal Baru: ";
                        getline(cin, jBaru.stasiunAsal);
                        cout << "Stasiun Tujuan Baru: ";
                        getline(cin, jBaru.stasiunTujuan);
                        cout << "Nama Kereta Baru: ";
                        getline(cin, jBaru.namaKereta);
                        while (true) {
                            cout << "Tanggal Baru (DD-MM-YYYY): ";
                            getline(cin, jBaru.tanggal);
                            if (sys.getJadwalManager().isValidTanggal(jBaru.tanggal)) break;
                            cout << "Format tanggal tidak valid. Harus DD-MM-YYYY.\n";
                        }
                        while (true) {
                            cout << "Waktu Berangkat Baru (JJ:MM): ";
                            getline(cin, jBaru.waktuBerangkat);
                            cout << "Waktu Tiba Baru (JJ:MM): ";
                            getline(cin, jBaru.waktuTiba);
                            if (sys.getJadwalManager().isValidWaktu(jBaru.waktuBerangkat) && sys.getJadwalManager().isValidWaktu(jBaru.waktuTiba)) break;
                            cout << "Format waktu tidak valid. Harus JJ:MM.\n";
                        }
                        // kode menggunakan kode lama
                        jBaru.kode = kode;
                        try {
                            sys.getJadwalManager().editJadwal(kode, jBaru);
                            cout << "Jadwal berhasil diedit dengan kode jadwal baru: " << jBaru.kode << endl;
                        } catch (const exception& e) {
                            cout << "Terjadi error: " << e.what() << "\n";
                        }
                        cout << "Tekan ENTER untuk melanjutkan...";
                        cin.get();
                        sys.simpanKeFile();
                    } else if (subChoice == 3) { 
                        header();
                        cin.ignore();
                        // Hapus Jadwal
                        string kode;
                        cout << "Masukkan kode jadwal yang akan dihapus: ";
                        getline(cin, kode);
                        try {
                            sys.getJadwalManager().hapusJadwal(kode);
                            cout << "Jadwal berhasil dihapus.\n";
                        } catch (const exception& e) {
                            cout << "Terjadi error: " << e.what() << "\n";
                        }
                        cout << "Tekan ENTER untuk melanjutkan...";
                        cin.get();
                        sys.simpanKeFile();
                    } else if (subChoice == 4) {
                        cout << "Kembali ke menu utama...\n";
                        break;
                    } else {
                        cout << "Pilihan tidak valid!\n";
                        break;
                    }
                } while (subChoice != 4);
                break;
            }
            case 2: { 
                header();
                cin.ignore();
                // Kelola Tiket Penumpang
                cout << "Kelola Tiket Penumpang:\n";
                cout << "1. Proses pemesanan tiket penumpang\n";
                cout << "2. Cek antrian pemesanan\n";
                cout << "3. Kembali\n";
                cout << "Pilih: ";
                int prosesPilihan;
                cin >> prosesPilihan;
                if (prosesPilihan == 1) {
                    sys.getTiketManager().prosesAntrianPesanan(); // Proses antrian tiket
                } else if (prosesPilihan == 2) {
                    sys.getTiketManager().cekAntrianPesanan(); // Lihat antrian tiket
                } else if (prosesPilihan == 3) {
                    cout << "Kembali ke menu utama...\n";
                    break;
                } else {
                    cout << "Pilihan tidak valid!\n";
                    break;
                }
                sys.simpanKeFile();
                break;
            }
            case 3: { 
                // Lihat Jadwal Kereta
                sys.muatDariFile();
                header();
                string tanggal;
                while (true) {
                        cout << "Masukkan tanggal (DD-MM-YYYY): ";
                        cin.ignore();
                        getline(cin, tanggal);
                        if (sys.getJadwalManager().isValidTanggal(tanggal)) break;
                        cout << "Format tanggal tidak valid. Harus DD-MM-YYYY.\n";
                }
                sys.getJadwalManager().tampilkanJadwal(tanggal);
                break;
            }
            case 4 : { // Kembali ke menu utama
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