#include "tiket.hpp"
#include "jadwal.hpp"
#include "file_io.hpp"
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

string TiketManager::generatePNR() const {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string pnr;
    for (int i = 0; i < 6; ++i) {
        pnr += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return pnr;
}

string TiketManager::generateBangku() const {
    int nomor = rand() % 100 + 1;
    char huruf = 'A' + (rand() % 4);
    return to_string(nomor) + huruf;
}

bool TiketManager::isSeatAvailable(const string& kodeJadwal, const string& seat) const {
    auto it = kursiTerpesan.find(kodeJadwal);
    if (it == kursiTerpesan.end()) return true;
    return it->second.find(seat) == it->second.end();
}

extern vector<Jadwal> daftarJadwal;
string TiketManager::cariJadwalByPesanan(
    const string& asal, const string& tujuan, const string& kereta, const string& tanggal) const
{
    auto it = find_if(daftarJadwal.begin(), daftarJadwal.end(), [&](const Jadwal& j) {
        return j.stasiunAsal == asal &&
               j.stasiunTujuan == tujuan &&
               j.namaKereta == kereta &&
               j.tanggal == tanggal;
    });
    if (it != daftarJadwal.end()) {
        return it->kode;
    }
    return "";
}

extern vector<Jadwal> daftarJadwal;
void TiketManager::tampilkanJadwalByKode(const string& kodeJadwal) const {
    auto it = find_if(daftarJadwal.begin(), daftarJadwal.end(), [&](const Jadwal& j) {
        return j.kode == kodeJadwal;
    });

    if (it != daftarJadwal.end()) {
        cout << "\nJadwal Kereta:\n"
             << "Nama Kereta     : " << it->namaKereta << "\n"
             << "Stasiun Asal    : " << it->stasiunAsal << "\n"
             << "Stasiun Tujuan  : " << it->stasiunTujuan << "\n"
             << "Waktu Berangkat : " << it->waktuBerangkat << "\n"
             << "Waktu Tiba      : " << it->waktuTiba << "\n";
    } else {
        cout << "Jadwal dengan kode kereta " << kodeJadwal << " tidak ditemukan.\n";
    }
}

void TiketManager::tambahKeAntrian(const Pemesanan& pemesanan) {
    antrianPemesanan.enqueue(pemesanan);
}

// Memproses antrian semua pemesanan tiket menggunakan queue
void TiketManager::prosesAntrianPesanan() {
    while (!antrianPemesanan.isEmpty()) {
        Pemesanan p = antrianPemesanan.peek();
        if (isSeatAvailable(p.kodeJadwal, p.nomorKursi)) {
            konfirmasiPemesanan.push(p);
        } else {
            cout << "Kursi " << p.nomorKursi << " pada kereta " << p.kodeJadwal << " sudah dipesan.\n";
        }
        antrianPemesanan.dequeue();
    }
}

extern vector<Jadwal> daftarJadwal;
void TiketManager::cekAntrianPesanan() const {
    if (antrianPemesanan.isEmpty()) {
        cout << "Antrian pemesanan kosong.\n";
        cout << "\nTekan ENTER untuk melanjutkan...";
        cin.ignore();
        cin.get();
    } else {
        cin.ignore();
        cout << "\nDaftar Antrian Pemesanan Tiket\n";
        cout << "+-----+----------------------+----------------------+--------------------------+-------------+-------------+------------+\n"
             << "| No  | Nama Penumpang       | Kode Jadwal          | Nama Kereta              | Berangkat   | Tiba        | Kursi      |\n"
             << "+-----+----------------------+----------------------+--------------------------+-------------+-------------+------------+\n";
        int no = 1;
        auto tempQueue = antrianPemesanan;
        while (!tempQueue.isEmpty()) {
            Pemesanan p = tempQueue.peek();
            string namaKereta = "-", waktuBerangkat = "-", waktuTiba = "-";
            // Cari data jadwal yang sesuai kodeJadwal di daftarJadwal
            for (const auto& j : daftarJadwal) {
                if (j.kode == p.kodeJadwal) {
                    namaKereta = j.namaKereta;
                    waktuBerangkat = j.waktuBerangkat;
                    waktuTiba = j.waktuTiba;
                    break;
                }
            }
            cout << "| " << left << setw(3)  << no++ << " "
                 << "| " << left << setw(21) << p.namaPenumpang
                 << "| " << left << setw(21) << p.kodeJadwal
                 << "| " << left << setw(25) << namaKereta
                 << "| " << left << setw(12) << waktuBerangkat
                 << "| " << left << setw(12) << waktuTiba
                 << "| " << left << setw(11) << p.nomorKursi << "|\n";
            tempQueue.dequeue();
        }
        cout << "+-----+----------------------+----------------------+--------------------------+-------------+-------------+------------+\n";
        cout << "\nTekan ENTER untuk melanjutkan...";
        cin.get();
    }
}

void TiketManager::tampilkanTiketByPNR(const string& pnr) const {
    auto it = find_if(daftarPemesanan.begin(), daftarPemesanan.end(), [&](const Pemesanan& p) {
        return p.pnr == pnr;
    });

    if (it != daftarPemesanan.end()) {
        cout << "\nTiket Ditemukan:\n"
             << "PNR         : " << it->pnr << "\n"
             << "Nama        : " << it->namaPenumpang << "\n"
             << "Kursi       : " << it->nomorKursi << "\n"
             << "Kode Kereta : " << it->kodeJadwal << "\n";
        tampilkanJadwalByKode(it->kodeJadwal);
    } else {
        cout << "Tiket dengan PNR " << pnr << " tidak ditemukan.\n";
    }
    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}

const vector<Pemesanan>& TiketManager::getDaftarPemesanan() const {
    return daftarPemesanan;
}
vector<Pemesanan>& TiketManager::getMutableDaftarPemesanan() {
    return daftarPemesanan;
}

const unordered_map<string, unordered_set<string>>& TiketManager::getKursiTerpesan() const {
    return kursiTerpesan;
}

unordered_map<string, unordered_set<string>>& TiketManager::getMutableKursiTerpesan() {
    return kursiTerpesan;
}

const Queue<Pemesanan>& TiketManager::getAntrianPesanan() const {
    return antrianPemesanan;
}
Queue<Pemesanan>& TiketManager::getMutableAntrian() {
    return antrianPemesanan;
}