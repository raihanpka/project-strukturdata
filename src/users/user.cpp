#include <menu.hpp>
#include <data.hpp>
#include <management.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

// Fungsi utama menu untuk user/penumpang
void menuUser(ManagementSystem& sys) {
    int choice;
    do {
        // Tampilkan judul dan menu
        header();
        cout << WHITE 
                << "Menu Penumpang:\n"
                << "1. Lihat Jadwal Kereta\n"
                << "2. Cari Data Penumpang berdasarkan PNR\n"
                << "3. Pesan Tiket\n"
                << "4. Kembali\n"
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
                sys.muatDariFile(); // Muat data terbaru dari file
                header();
                string tanggal;
                while (true) {
                    cin.ignore(); // Buang newline sebelumnya
                    cout << "Masukkan Tanggal (DD-MM-YYYY): ";
                    getline(cin, tanggal);
                    if (sys.getJadwalManager().isValidTanggal(tanggal)) {
                        break;
                    } else {
                        cout << "Format tanggal tidak valid. Harus DD-MM-YYYY.\n";
                    }
                }
                sys.getJadwalManager().tampilkanJadwal(tanggal); // Tampilkan jadwal sesuai tanggal
                break;
            }
            case 2: {
                sys.muatDariFile(); // Muat data terbaru dari file
                header();
                cin.ignore();
                string pnr;
                cout << "Masukkan PNR Tiket: ";
                getline(cin, pnr);
                if (pnr.empty()) {
                    cout << "PNR tidak boleh kosong!\n";
                    cout << "Tekan ENTER untuk melanjutkan...";
                    cin.get();
                    continue;
                }
                sys.getTiketManager().tampilkanTiketByPNR(pnr); // Cari dan tampilkan tiket
                break;
            }
            case 3: {
                header();
                Stack<Pemesanan> undoStack;
                Pemesanan p;
                bool selesaiInput = false;
                while (!selesaiInput) {
                    cin.ignore();
                    p = Pemesanan();
                    vector<Jadwal> daftar = sys.getJadwalManager().getJadwal();
                    if (daftar.empty()) {
                        cout << "Belum ada jadwal tersedia.\n";
                        break;
                    }
                    cout << "Cari jadwal berdasarkan:\n";
                    cout << "1. Stasiun Asal\n";
                    cout << "2. Stasiun Tujuan\n";
                    cout << "3. Nama Kereta\n";
                    cout << "Pilihan: ";
                    int mode;
                    cin >> mode;
                    string hasilPilihan;
                    vector<Jadwal> hasil;
                    if (mode == 1) hasil = sys.getTiketManager().tampilkanPilihanJadwal(daftar, "Stasiun Asal", hasilPilihan);
                    else if (mode == 2) hasil = sys.getTiketManager().tampilkanPilihanJadwal(daftar, "Stasiun Tujuan", hasilPilihan);
                    else if (mode == 3) hasil = sys.getTiketManager().tampilkanPilihanJadwal(daftar, "Kereta", hasilPilihan);
                    else {
                        cout << "Pilihan tidak valid!\n";
                        continue;
                    }
                    if (hasil.empty()) {
                        cout << "Tidak ada jadwal untuk pilihan tersebut.\n";
                        continue;
                    }

                    // Tampilkan semua jadwal hasil filter dan pilih salah satu
                    cout << "\nDaftar Jadwal Tersedia:\n";
                    cout << left
                         << setw(5)  << "No"
                         << setw(25) << "Nama Kereta"
                         << setw(20) << "Stasiun Asal"
                         << setw(20) << "Stasiun Tujuan"
                         << setw(15) << "Tanggal"
                         << setw(12) << "Berangkat"
                         << setw(12) << "Tiba"
                         << setw(15) << "Kode"
                         << endl;
                    cout << string(124, '-') << endl;
                    for (size_t i = 0; i < hasil.size(); ++i) {
                        cout << setw(5)  << i+1
                             << setw(25) << hasil[i].namaKereta
                             << setw(20) << hasil[i].stasiunAsal
                             << setw(20) << hasil[i].stasiunTujuan
                             << setw(15) << hasil[i].tanggal
                             << setw(12) << hasil[i].waktuBerangkat
                             << setw(12) << hasil[i].waktuTiba
                             << setw(15) << hasil[i].kode
                             << endl;
                    }
                    cout << "\nPilih jadwal (nomor): ";
                    cin.ignore();
                    int idxJadwal;
                    cin >> idxJadwal;
                    if (idxJadwal < 1 || idxJadwal > (int)hasil.size()) {
                        cout << "\nPilihan tidak valid!\n";
                        continue;
                    }
                    Jadwal jadwalDipilih = hasil[idxJadwal-1];

                    // Input nama penumpang
                    cout << "Nama Penumpang: ";
                    getline(cin, p.namaPenumpang);

                    p.kodeJadwal = jadwalDipilih.kode;
                    sys.getTiketManager().tampilkanJadwalByKode(p.kodeJadwal);
                    p.nomorKursi = sys.getTiketManager().generateBangku();
                    cout << "\nNomor Kursi     : " << p.nomorKursi << "\n";
                    p.pnr = sys.getTiketManager().generatePNR();
                    cout << "\nPNR yang digenerate: " << p.pnr << "\n";

                    undoStack.push(p);

                    cout << "\nLanjutkan input? (tekan ENTER untuk lanjut, atau ketik 0 untuk undo): ";
                    string aksi;
                    getline(cin, aksi);
                    if (aksi == "0" && !undoStack.isEmpty()) {
                        undoStack.pop();
                        cout << "Input terakhir dibatalkan. Silakan ENTER untuk input ulang!\n";
                        continue;
                    } else {
                        selesaiInput = true;
                        Pemesanan pesananFinal = undoStack.peek();
                        if (!sys.getTiketManager().isSeatAvailable(pesananFinal.kodeJadwal, pesananFinal.nomorKursi)) {
                            cout << "Kursi tidak tersedia!\n";
                        } else {
                            sys.getTiketManager().tambahKeAntrian(pesananFinal);
                            cout << "\nPesanan telah masuk ke antrian.\n";
                        }
                        cout << "Tekan ENTER untuk melanjutkan...\n";
                        cin.get();
                        sys.simpanKeFile();
                    }
                }
                break;
            }
            case 4: {
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