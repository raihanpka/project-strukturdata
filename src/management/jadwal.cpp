#include "jadwal.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <stdexcept>
#include <ctime>

using namespace std;

bool JadwalManager::isValidTanggal(const string& tanggal) {
    tm tm = {};
    istringstream ss(tanggal);
    ss >> get_time(&tm, "%d-%m-%Y");
    return !ss.fail();
}

bool JadwalManager::isValidWaktu(const string& waktu) {
    tm tm = {};
    istringstream ss(waktu);
    ss >> get_time(&tm, "%H:%M");
    return !ss.fail();
}

string JadwalManager::generateKodeJadwal(const string& namaKereta, const string& stasiunAsal,
                                         const string& stasiunTujuan, const string& tanggal) 
{
    string kode = "KA-";
    int count = 0;
    for (char c : namaKereta) {
        if (count >= 3) break;
        if (isalpha(c)) {
            kode += toupper(c);
            count++;
        }
    }
    while (count < 3) kode += 'X', count++;
    if (!stasiunAsal.empty()) kode += toupper(stasiunAsal[0]);
    if (!stasiunTujuan.empty()) kode += toupper(stasiunTujuan[0]);

    // Asumsikan tanggal sudah valid sebelum masuk sini
    tm tm = {};
    istringstream ss(tanggal);
    ss >> get_time(&tm, "%d-%m-%Y");

    char day[3], month[3];
    strftime(day, sizeof(day), "%d", &tm);
    strftime(month, sizeof(month), "%m", &tm);
    kode += string(day) + month;

    int locomotiveNum = 1;
    string baseKode = kode;
    for (const auto& j : daftarJadwal) {
        if (j.kode.find(baseKode) == 0) {
            size_t locPos = j.kode.find("L");
            if (locPos != string::npos) {
                int existingNum = stoi(j.kode.substr(locPos + 1));
                if (existingNum >= locomotiveNum) locomotiveNum = existingNum + 1;
            }
        }
    }
    return kode + "L" + to_string(locomotiveNum);
}

void JadwalManager::tambahJadwal(const Jadwal& jadwal) {
    if (jadwal.stasiunAsal.empty() || jadwal.stasiunTujuan.empty() ||
        jadwal.namaKereta.empty() || jadwal.tanggal.empty() ||
        jadwal.waktuBerangkat.empty() || jadwal.waktuTiba.empty()) {
        throw invalid_argument("Semua data jadwal harus diisi.");
    }
    daftarJadwal.push_back(jadwal);
    sortSchedules();
}

void JadwalManager::tampilkanJadwal(const string& filterTanggal) const {
    vector<Jadwal> filtered;
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
        // Parsing tanggal dalam format DD-MM-YYYY
        tm tm = {};
        istringstream ss(filterTanggal);
        ss >> get_time(&tm, "%d-%m-%Y");
        if (ss.fail()) {
            throw runtime_error("Format tanggal tidak valid: " + filterTanggal);
        }
        
        // Konversi nomor bulan ke nama bulan
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
    cout << "\nJadwal Kereta Api Indonesia" << headerDate << "\n";
    cout << "+-----------------+----------------------+----------------------+--------------------------+-------------+-------------+\n"
         << "| Kode            | Stasiun Asal         | Stasiun Tujuan       | Nama Kereta              | Berangkat   | Tiba        |\n"
         << "+-----------------+----------------------+----------------------+--------------------------+-------------+-------------+\n";

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

void JadwalManager::sortSchedules() {
    sort(daftarJadwal.begin(), daftarJadwal.end(),
        [](const Jadwal& a, const Jadwal& b) {
            if (a.tanggal != b.tanggal) return a.tanggal < b.tanggal;
            return a.waktuBerangkat < b.waktuBerangkat;
        });
}

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
             << "Konfirmasi (Y/N)? ";
        char input;
        cin >> input;
        input = toupper(input);
        cin.ignore();

        if (input == 'Y' || input == 'y') {
            daftarJadwal.push_back(j);
            konfirmasiJadwal.pop();
        } else {
            konfirmasiJadwal.pop();
        }
    }
}

const vector<Jadwal>& JadwalManager::getJadwal() const {
    return daftarJadwal;
}

// Tambahkan akses non-const ke daftarJadwal
vector<Jadwal>& JadwalManager::getDaftarJadwal() {
    return daftarJadwal;
}
