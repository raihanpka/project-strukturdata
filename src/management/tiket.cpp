#include "tiket.hpp"
#include "jadwal.hpp"
#include "file_io.hpp"
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

// Generate kode PNR acak 6 karakter (huruf/angka)
string TiketManager::generatePNR() const {
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string pnr;
    for (int i = 0; i < 6; ++i) {
        pnr += alphanum[rand() % (sizeof(alphanum) - 1)]; // Pilih karakter acak
    }
    return pnr;
}

// Generate nomor bangku acak (1-100) dan huruf A-D
string TiketManager::generateBangku() const {
    int nomor = rand() % 100 + 1; // Nomor kursi 1-100
    char huruf = 'A' + (rand() % 4); // Huruf A-D
    return to_string(nomor) + huruf;
}

// Mengecek apakah kursi tersedia untuk kode jadwal tertentu
bool TiketManager::isSeatAvailable(const string& kodeJadwal, const string& seat) const {
    auto it = kursiTerpesan.find(kodeJadwal);
    if (it == kursiTerpesan.end()) return true; // Jika belum ada kursi terpesan, berarti tersedia
    return it->second.find(seat) == it->second.end(); // True jika kursi belum dipesan
}

// Menampilkan pilihan jadwal berdasarkan field tertentu (asal, tujuan, kereta, tanggal)
vector<Jadwal> TiketManager::tampilkanPilihanJadwal(const vector<Jadwal>& daftar, const string& field, string& hasilPilihan) {
    vector<string> opsi;
    vector<Jadwal> hasil;
    // Kumpulkan opsi unik sesuai field
    for (const auto& j : daftar) {
        string val;
        if (field == "Stasiun Asal") val = j.stasiunAsal;
        else if (field == "Stasiun Tujuan") val = j.stasiunTujuan;
        else if (field == "Kereta") val = j.namaKereta;
        if (find(opsi.begin(), opsi.end(), val) == opsi.end()) opsi.push_back(val);
    }
    // Tampilkan opsi ke user
    cout << "\nMasukkan " << field << ":\n";
    for (size_t i = 0; i < opsi.size(); ++i) {
        cout << i+1 << ". " << opsi[i] << endl;
    }
    cout << "\nPilih opsi: ";
    int idx;
    cin >> idx;
    cin.ignore();
    if (idx < 1 || idx > (int)opsi.size()) {
        cout << "Pilihan tidak valid!\n";
        return {};
    }
    hasilPilihan = opsi[idx-1];
    // Filter jadwal sesuai pilihan
    for (const auto& j : daftar) {
        if ((field == "Stasiun Asal" && j.stasiunAsal == hasilPilihan) ||
            (field == "Stasiun Tujuan" && j.stasiunTujuan == hasilPilihan) ||
            (field == "Kereta" && j.namaKereta == hasilPilihan)) {
            hasil.push_back(j);
        }
    }
    return hasil;
}

// Mencari kode jadwal berdasarkan data pesanan (asal, tujuan, kereta, tanggal)
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
        return it->kode; // Kembalikan kode jika ditemukan
    }
    return ""; // Jika tidak ditemukan, kembalikan string kosong
}

// Menampilkan detail jadwal berdasarkan kode jadwal
extern vector<Jadwal> daftarJadwal;
void TiketManager::tampilkanJadwalByKode(const string& kodeJadwal) const {
    auto it = find_if(daftarJadwal.begin(), daftarJadwal.end(), [&](const Jadwal& j) {
        return j.kode == kodeJadwal;
    });

    if (it != daftarJadwal.end()) {
        cout << "\nJadwal Kereta\n"
             << "Nama Kereta     : " << it->namaKereta << "\n"
             << "Stasiun Asal    : " << it->stasiunAsal << "\n"
             << "Stasiun Tujuan  : " << it->stasiunTujuan << "\n"
             << "Waktu Berangkat : " << it->waktuBerangkat << "\n"
             << "Waktu Tiba      : " << it->waktuTiba << "\n";
    } else {
        cout << "Jadwal dengan kode " << kodeJadwal << " tidak ditemukan.\n";
    }
}

// Menambah pesanan ke antrian (queue)
void TiketManager::tambahKeAntrian(const Pemesanan& pemesanan) {
    antrianPemesanan.enqueue(pemesanan);
}

// Memproses seluruh antrian pemesanan tiket
void TiketManager::prosesAntrianPesanan() {
    cin.ignore();
    cout << "\nMemproses antrian pemesanan tiket...\n";
    while (!antrianPemesanan.isEmpty()) {
        Pemesanan p = antrianPemesanan.peek(); // Ambil pesanan paling depan
        if (isSeatAvailable(p.kodeJadwal, p.nomorKursi)) {
            p.confirmed = 1; // Tandai sudah dikonfirmasi
            daftarPemesanan.push_back(p); // Masukkan ke daftar pemesanan
            cout << "Kursi " << p.nomorKursi << " pada kode " << p.kodeJadwal
                 << " berhasil dipesan untuk penumpang " << p.namaPenumpang << ".\n";
        } else {
            cout << "Kursi " << p.nomorKursi << " pada kode " << p.kodeJadwal << " sudah dipesan.\n";
            cout << "Pesanan untuk penumpang " << p.namaPenumpang << " tidak dapat dilanjutkan.\n";
            // Ulangi generate kursi baru sampai dapat yang tersedia maksimal 10 kali
            const int maxPercobaan = 10;
            int percobaan = 0;
            bool berhasil = false;
            while (percobaan < maxPercobaan) {
                p.nomorKursi = generateBangku(); // Generate kursi baru
                cout << "Kursi baru yang dialokasikan: " << p.nomorKursi << "\n";
                if (isSeatAvailable(p.kodeJadwal, p.nomorKursi)) {
                    p.confirmed = 1; // Tandai sudah dikonfirmasi
                    daftarPemesanan.push_back(p); // Masukkan ke daftar pemesanan
                    cout << "Kursi " << p.nomorKursi << " berhasil dipesan untuk penumpang " << p.namaPenumpang << ".\n";
                    berhasil = true;
                    break;
                } else {
                    cout << "Kursi " << p.nomorKursi << " juga sudah dipesan. Mencoba lagi...\n";
                }
                ++percobaan;
            }
            if (!berhasil) {
                cout << "Tidak dapat menemukan kursi yang tersedia setelah " << maxPercobaan << " percobaan. Pesanan dibatalkan.\n";
            }
        }
        antrianPemesanan.dequeue(); // Hapus dari antrian
    }
    cout << "Semua antrian pemesanan telah diproses.\n";
    cout << "Tekan ENTER untuk melanjutkan...";
    cin.get();
}

// Menampilkan seluruh antrian pemesanan tiket
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
        auto curr = antrianPemesanan.getFront(); // Mulai dari node depan queue
        while (curr != nullptr) {
            Pemesanan p = curr->data;
            string namaKereta = "-", waktuBerangkat = "-", waktuTiba = "-";
            // Cari detail jadwal berdasarkan kode
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
            curr = curr->next; // Lanjut ke node berikutnya
        }
        cout << "+-----+----------------------+----------------------+--------------------------+-------------+-------------+------------+\n";
        cout << "\nTekan ENTER untuk melanjutkan...";
        cin.get();
    }
}

// Menampilkan tiket berdasarkan PNR
void TiketManager::tampilkanTiketByPNR(const string& pnr) const {
    auto it = find_if(daftarPemesanan.begin(), daftarPemesanan.end(), [&](const Pemesanan& p) {
        return p.pnr == pnr;
    });

    if (it != daftarPemesanan.end()) {
        cout << "\nTiket Ditemukan:\n"
             << "PNR         : " << it->pnr << "\n"
             << "Nama        : " << it->namaPenumpang << "\n"
             << "Kursi       : " << it->nomorKursi << "\n"
             << "Kode Jadwal : " << it->kodeJadwal << "\n";
        tampilkanJadwalByKode(it->kodeJadwal); // Tampilkan detail jadwal
    } else {
        cout << "Tiket dengan PNR " << pnr << " tidak ditemukan.\n";
    }
    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}

// Getter daftar pemesanan (read-only)
const vector<Pemesanan>& TiketManager::getDaftarPemesanan() const {
    return daftarPemesanan;
}
// Getter daftar pemesanan (mutable)
vector<Pemesanan>& TiketManager::getMutableDaftarPemesanan() {
    return daftarPemesanan;
}

// Getter kursi terpesan (read-only)
const unordered_map<string, unordered_set<string>>& TiketManager::getKursiTerpesan() const {
    return kursiTerpesan;
}
// Getter kursi terpesan (mutable)
unordered_map<string, unordered_set<string>>& TiketManager::getMutableKursiTerpesan() {
    return kursiTerpesan;
}

// Getter antrian pesanan (read-only)
const Queue<Pemesanan>& TiketManager::getAntrianPesanan() const {
    return antrianPemesanan;
}
// Getter antrian pesanan (mutable)
Queue<Pemesanan>& TiketManager::getMutableAntrian() {
    return antrianPemesanan;
}