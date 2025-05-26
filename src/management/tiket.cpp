#include "tiket.hpp"
#include <cstdlib>
#include <algorithm>
#include <cctype>

using namespace std;

string TiketManager::generatePNR() const {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string pnr;
    for (int i = 0; i < 6; ++i) {
        pnr += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return pnr;
}

bool TiketManager::isSeatAvailable(const string& kodeKereta, const string& seat) const {
    auto it = kursiTerpesan.find(kodeKereta);
    if (it == kursiTerpesan.end()) return true;
    return it->second.find(seat) == it->second.end();
}


void TiketManager::tambahKeAntrian(const Pemesanan& pemesanan) {
    antrianPemesanan.enqueue(pemesanan);
}

// Memproses antrian pemesanan tiket menggunakan queue
void TiketManager::prosesAntrianPesanan() {
    while (!antrianPemesanan.isEmpty()) {
        Pemesanan p = antrianPemesanan.peek();
        if (isSeatAvailable(p.kodeKereta, p.nomorKursi)) {
            konfirmasiPemesanan.push(p);
        } else {
            cout << "Kursi " << p.nomorKursi << " pada kereta " << p.kodeKereta << " sudah dipesan.\n";
        }
        antrianPemesanan.dequeue();
    }
}

// Menggunakan stack untuk konfirmasi pemesanan
void TiketManager::prosesKonfirmasiPemesanan() {
    while (!konfirmasiPemesanan.isEmpty()) {
        Pemesanan p = konfirmasiPemesanan.peek();
        cout << "\nKonfirmasi Pemesanan:\n"
             << "PNR         : " << p.pnr << "\n"
             << "Nama        : " << p.namaPenumpang << "\n"
             << "Kursi       : " << p.nomorKursi << "\n"
             << "Kode Kereta : " << p.kodeKereta << "\n"
             << "Konfirmasi (Y/N)? ";
        char input;
        cin >> input;
        input = toupper(input);
        cin.ignore();

        if (input == 'Y' || input == 'y') {
            p.confirmed = true;
            daftarPemesanan.push_back(p);
            kursiTerpesan[p.kodeKereta].insert(p.nomorKursi);
        }
        konfirmasiPemesanan.pop();
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
             << "Kode Kereta : " << it->kodeKereta << "\n";
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
