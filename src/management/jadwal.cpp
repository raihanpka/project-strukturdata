#include "jadwal.hpp"
#include "sorting.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <ctime>

using namespace std;

vector<Jadwal> daftarJadwal; // Menyimpan seluruh jadwal kereta

// Mengecek apakah format tanggal valid (DD-MM-YYYY)
bool JadwalManager::isValidTanggal(const string& tanggal) {
    tm tm = {};
    istringstream ss(tanggal);
    ss >> get_time(&tm, "%d-%m-%Y");
    return !ss.fail(); // True jika parsing sukses
}

// Mengecek apakah format waktu valid (HH:MM)
bool JadwalManager::isValidWaktu(const string& waktu) {
    tm tm = {};
    istringstream ss(waktu);
    ss >> get_time(&tm, "%H:%M");
    return !ss.fail(); // True jika parsing sukses
}

// Membuat kode jadwal unik berdasarkan data input
string JadwalManager::hashingKodeJadwal(const string& namaKereta, const string& stasiunAsal,
                                         const string& stasiunTujuan, const string& tanggal) 
{
    string kode = "KAI"; // Prefix kode
    int count = 0;
    // Ambil 3 huruf pertama dari nama kereta
    for (char c : namaKereta) {
        if (count >= 3) break;
        if (isalpha(c)) {
            kode += toupper(c);
            count++;
        }
    }
    while (count < 3) kode += 'X', count++; // Jika kurang dari 3, tambahkan 'X'
    if (!stasiunAsal.empty()) kode += toupper(stasiunAsal[0]); // Inisial asal
    if (!stasiunTujuan.empty()) kode += toupper(stasiunTujuan[0]); // Inisial tujuan

    // Ambil tanggal (asumsi sudah valid)
    tm tm = {};
    istringstream ss(tanggal);
    ss >> get_time(&tm, "%d-%m-%Y");

    char day[3], month[3];
    strftime(day, sizeof(day), "%d", &tm);
    strftime(month, sizeof(month), "%m", &tm);
    kode += string(day) + month; // Tambahkan hari dan bulan

    int locomotiveNum = 1;
    string baseKode = kode;
    // Cek jika sudah ada kode serupa, tambahkan nomor urut
    for (const auto& j : daftarJadwal) {
        if (j.kode.find(baseKode) == 0) {
            size_t locPos = j.kode.find("L");
            if (locPos != string::npos) {
                int existingNum = stoi(j.kode.substr(locPos + 1));
                if (existingNum >= locomotiveNum) locomotiveNum = existingNum + 1;
            }
        }
    }
    return kode + "L" + to_string(locomotiveNum); // Kode akhir
}

// Menambah jadwal baru ke daftar dan langsung sort
void JadwalManager::tambahJadwal(const Jadwal& jadwal) {
    // Validasi data tidak boleh kosong
    if (jadwal.stasiunAsal.empty() || jadwal.stasiunTujuan.empty() ||
        jadwal.namaKereta.empty() || jadwal.tanggal.empty() ||
        jadwal.waktuBerangkat.empty() || jadwal.waktuTiba.empty()) {
        throw invalid_argument("Semua data jadwal harus diisi.");
    }
    daftarJadwal.push_back(jadwal); // Tambah ke vector
    sortSchedules(); // Urutkan jadwal
}

// Edit jadwal berdasarkan kode
void JadwalManager::editJadwal(const string& kodeJadwal, const Jadwal& jadwalBaru) {
    auto it = find_if(daftarJadwal.begin(), daftarJadwal.end(),
                      [&kodeJadwal](const Jadwal& j) { return j.kode == kodeJadwal; });
    if (it != daftarJadwal.end()) {
        *it = jadwalBaru; // Ganti data
        sortSchedules();  // Urutkan ulang
    } else {
        throw runtime_error("Jadwal dengan kode " + kodeJadwal + " tidak ditemukan.");
    }
}

// Hapus jadwal berdasarkan kode, simpan ke stack konfirmasi
void JadwalManager::hapusJadwal(const string& kodeJadwal) {
    auto it = find_if(daftarJadwal.begin(), daftarJadwal.end(),
                      [&kodeJadwal](const Jadwal& j) { return j.kode == kodeJadwal; });
    if (it != daftarJadwal.end()) {
        konfirmasiJadwal.push(*it); // Simpan ke stack konfirmasi
        daftarJadwal.erase(it);     // Hapus dari vector
    } else {
        throw runtime_error("Jadwal dengan kode " + kodeJadwal + " tidak ditemukan.");
    }
}

// Menampilkan jadwal, bisa difilter berdasarkan tanggal
void JadwalManager::tampilkanJadwal(const string& filterTanggal) const {
    vector<Jadwal> filtered;
    // Filter jadwal sesuai tanggal jika ada
    for (const auto& j : daftarJadwal) {
        if (filterTanggal.empty() || j.tanggal == filterTanggal)
            filtered.push_back(j);
    }
    if (filtered.empty()) {
        cout << "\nTidak ada jadwal yang tersedia";
        if (!filterTanggal.empty()) cout << " untuk tanggal " << filterTanggal;
        cout << "\nTekan ENTER untuk melanjutkan...";
        cin.get();
        return;
    }

    string headerDate;
    if (!filterTanggal.empty()) {
        // Parsing tanggal untuk header
        tm tm = {};
        istringstream ss(filterTanggal);
        ss >> get_time(&tm, "%d-%m-%Y");
        if (ss.fail()) {
            throw runtime_error("Format tanggal tidak valid: " + filterTanggal);
        }
        
        // Nama bulan dalam bahasa Indonesia
        const char* monthNames[] = {
            "Januari", "Februari", "Maret", "April", "Mei", "Juni",
            "Juli", "Agustus", "September", "Oktober", "November", "Desember"
        };
        
        char day[3];
        strftime(day, sizeof(day), "%d", &tm);
        int month = tm.tm_mon;
        int year = tm.tm_year + 1900;
        
        headerDate = " - " + string(day) + " " + monthNames[month] + " " + to_string(year);
    }
    // Tampilkan header tabel
    cout << "\nJadwal Kereta Api Indonesia" << headerDate << "\n";
    cout << "+-----------------+----------------------+----------------------+--------------------------+-------------+-------------+\n"
         << "| Kode            | Stasiun Asal         | Stasiun Tujuan       | Nama Kereta              | Berangkat   | Tiba        |\n"
         << "+-----------------+----------------------+----------------------+--------------------------+-------------+-------------+\n";

    // Tampilkan setiap jadwal
    for (const auto& j : filtered) {
        cout << "| " << left << setw(15) << j.kode << " | "
             << setw(20) << j.stasiunAsal << " | "
             << setw(20) << j.stasiunTujuan << " | "
             << setw(24) << j.namaKereta << " | "
             << setw(11) << j.waktuBerangkat << " | "
             << setw(11) << j.waktuTiba << " |\n";
    }
    cout << "+-----------------+----------------------+----------------------+--------------------------+-------------+-------------+\n";
    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.get();
}

// Mengubah tanggal ke format YYYYMMDD agar mudah diurutkan
std::string toSortableDate(const std::string& tanggal) {
    if (tanggal.size() != 10) return tanggal;
    return tanggal.substr(6,4) + tanggal.substr(3,2) + tanggal.substr(0,2);
}

// Mengubah waktu ke format HHMM agar mudah diurutkan
std::string toSortableTime(const std::string& waktu) {
    if (waktu.size() != 5) return waktu;
    return waktu.substr(0,2) + waktu.substr(3,2);
}

// Functor untuk membandingkan dua jadwal berdasarkan tanggal dan waktu
struct CompareJadwal {
    bool operator()(const Jadwal& a, const Jadwal& b) const {
        std::string tglA = toSortableDate(a.tanggal);
        std::string tglB = toSortableDate(b.tanggal);
        if (tglA != tglB) return tglA < tglB;
        return toSortableTime(a.waktuBerangkat) < toSortableTime(b.waktuBerangkat);
    }
};

// Mengurutkan daftar jadwal dengan quicksort dan CompareJadwal
void JadwalManager::sortSchedules() {
    if (!daftarJadwal.empty()) {
        quicksort<Jadwal>(daftarJadwal, 0, static_cast<int>(daftarJadwal.size()) - 1, CompareJadwal());
    }
}

// Memproses stack konfirmasi jadwal (admin harus konfirmasi sebelum jadwal aktif)
void JadwalManager::prosesKonfirmasiJadwal() {
    while(!konfirmasiJadwal.isEmpty()) {
        Jadwal j = konfirmasiJadwal.peek();
        cout << "\nKonfirmasi Jadwal:\n"
             << "Kode         : " << j.kode << "\n"
             << "Stasiun Asal : " << j.stasiunAsal << "\n"
             << "Stasiun Tujuan: " << j.stasiunTujuan << "\n"
             << "Nama Kereta  : " << j.namaKereta << "\n"
             << "Tanggal      : " << j.tanggal << "\n"
             << "Waktu Berangkat: " << j.waktuBerangkat << "\n"
             << "Waktu Tiba   : " << j.waktuTiba << "\n"
             << "\nKonfirmasi (Y/N)? ";
        char input;
        cin >> input;
        input = toupper(input);
        cin.ignore();

        if (input == 'Y' || input == 'y') {
            daftarJadwal.push_back(j); // Jika dikonfirmasi, masukkan ke daftar
            konfirmasiJadwal.pop();    // Hapus dari stack konfirmasi
        } else {
            konfirmasiJadwal.pop();    // Jika tidak, tetap hapus dari stack
        }
    }
}

// Mengambil daftar jadwal (const)
const vector<Jadwal>& JadwalManager::getJadwal() const {
    return daftarJadwal;
}

// Mengambil daftar jadwal (non-const, bisa diubah)
vector<Jadwal>& JadwalManager::getDaftarJadwal() {
    return daftarJadwal;
}