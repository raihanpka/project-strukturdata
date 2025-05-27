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
                << "1. Lihat Jadwal Kereta\n"
                << "2. Cari Tiket Penumpang\n"
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
                sys.muatDariFile();
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
                sys.muatDariFile();
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
                Stack<Pemesanan> undoStack;
                Pemesanan p;
                bool selesaiInput = false;

                auto inputField = [](const string& prompt, string& field) {
                    cout << prompt;
                    getline(cin, field);
                    if (field == "0") return -1;
                    if (field == "1") return 1;
                    return 0;
                };

                while (!selesaiInput) {
                    p = Pemesanan(); // Reset data
                    string stasiunAsal, stasiunTujuan, kereta, tanggal;
                    while (true) {
                        int res;
                        cin.ignore();
                        res = inputField("Nama Penumpang: ", p.namaPenumpang);
                        if (res == -1) { cout << "Tidak ada data untuk undo.\n"; continue; }
                        if (res == 1) break;

                        res = inputField("Stasiun Asal: ", stasiunAsal);
                        if (res == -1) { cout << "Undo ke input sebelumnya.\n"; continue; }
                        if (res == 1) break;

                        res = inputField("Stasiun Tujuan: ", stasiunTujuan);
                        if (res == -1) { cout << "Undo ke input sebelumnya.\n"; continue; }
                        if (res == 1) break;

                        res = inputField("Nama Kereta: ", kereta);
                        if (res == -1) { cout << "Undo ke input sebelumnya.\n"; continue; }
                        if (res == 1) break;
                        
                        while (true) {
                            res = inputField("Tanggal Keberangkatan (DD-MM-YYYY): ", tanggal);
                            if (sys.getJadwalManager().isValidTanggal(tanggal)) break;
                            cout << "Format tanggal tidak valid. Harus DD-MM-YYYY.\n";
                            if (res == -1) { cout << "Undo ke input sebelumnya.\n"; continue; }
                            if (res == 1) break;
                        }

                        break;
                    }

                    p.kodeJadwal = sys.getTiketManager().cariJadwalByPesanan(stasiunAsal, stasiunTujuan, kereta, tanggal);
                    if (p.kodeJadwal.empty()) {
                        cout << "Tidak ada jadwal yang ditemukan untuk input tersebut.\n";
                        continue;
                    }

                    sys.getTiketManager().tampilkanJadwalByKode(p.kodeJadwal);
                    p.nomorKursi = sys.getTiketManager().generateBangku();
                    cout << "Nomor Kursi     : " << p.nomorKursi << "\n";
                    p.pnr = sys.getTiketManager().generatePNR();
                    cout << "\nPNR yang digenerate: " << p.pnr << "\n";

                    // Simpan ke stack undo
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
                        // Hanya masukkan ke antrian di sini!
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