// Implementasi sistem manajemen kereta api

#include "../include/management/management.hpp"
#include "../include/file_path.hpp"
#include <map>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>

bool ManagementSystem::authenticateAdmin(const std::string& username, const std::string& password) {
    std::ifstream file(ADMIN_CREDENTIALS_FILE);
    std::string line;
    while (std::getline(file, line)) {
        size_t delimiter = line.find(':');
        if (delimiter != std::string::npos) {
            std::string fileUsername = line.substr(0, delimiter);
            std::string filePassword = line.substr(delimiter + 1);
            if (username == fileUsername && password == filePassword) {
                currentUserRole = UserRole::ADMIN;
                currentUsername = username;
                return true;
            }
        }
    }
    return false;
}

void ManagementSystem::setPassengerMode() {
    currentUserRole = UserRole::PASSENGER;
    currentUsername = "Guest";
}

std::string ManagementSystem::generateKodeJadwal(const std::string& namaKereta, const std::string& stasiunAsal,
                                               const std::string& stasiunTujuan, const std::string& tanggal) {
    std::string kode = "KA-";
    
    // Ambil 3 huruf pertama nama kereta
    int count = 0;
    for(char c : namaKereta) {
        if(count >= 3) break;
        if(isalpha(c)) {
            kode += toupper(c);
            count++;
        }
    }
    // Tambahkan X jika kurang dari 3 huruf
    while(count < 3) {
        kode += 'X';
        count++;
    }
    
    // Tambahkan huruf pertama stasiun asal dan tujuan
    if (!stasiunAsal.empty()) kode += toupper(stasiunAsal[0]);
    if (!stasiunTujuan.empty()) kode += toupper(stasiunTujuan[0]);
    
    // Parsing tanggal dalam format DD-MM-YYYY
    std::tm tm = {};
    std::istringstream ss(tanggal);
    ss >> std::get_time(&tm, "%d-%m-%Y");
    if (ss.fail()) {
        throw std::runtime_error("Error pada inputan tanggal: " + tanggal);
    }
    
    // Format komponen tanggal (DDMM)
    char day[3], month[3];
    strftime(day, sizeof(day), "%d", &tm);
    strftime(month, sizeof(month), "%m", &tm);
    kode += std::string(day) + month;
    
    // Cek duplikat dan tambahkan nomor lokomotif
    int locomotiveNum = 1;
    std::string baseKode = kode;
    for (const auto& j : jadwalKereta) {
        if (j.kode.find(baseKode) == 0) {
            size_t locPos = j.kode.find("L");
            if (locPos != std::string::npos) {
                int existingNum = std::stoi(j.kode.substr(locPos + 1));
                if (existingNum >= locomotiveNum) {
                    locomotiveNum = existingNum + 1;
                }
            }
        }
    }
    
    return kode + "L" + std::to_string(locomotiveNum);
}

void ManagementSystem::tampilkanJadwal(const std::string& filterTanggal) const {
    std::vector<Jadwal> filtered;
    if (!filterTanggal.empty()) {
    // Filter jadwal berdasarkan tanggal
        for (const auto& j : jadwalKereta) {
            if (j.tanggal == filterTanggal) {
                filtered.push_back(j);
            }
        }
    } else {
        filtered = jadwalKereta;
    }

    if (filtered.empty()) {
        std::cout << "\nTidak ada jadwal yang tersedia";
        if (!filterTanggal.empty()) {
            std::cout << " untuk tanggal " << filterTanggal;
        }
        std::cout << "\n";
        return;
    }

    std::string headerDate;
    if (!filterTanggal.empty()) {
    // Parsing tanggal dalam format DD-MM-YYYY
    std::tm tm = {};
    std::istringstream ss(filterTanggal);
    ss >> std::get_time(&tm, "%d-%m-%Y");
        if (ss.fail()) {
            throw std::runtime_error("Format tanggal tidak valid: " + filterTanggal);
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
        
        headerDate = " - " + std::string(day) + " " + monthNames[month] + " " + std::to_string(year);
    }
    std::cout << "\nJadwal Kereta API" << headerDate << "\n";
    std::cout << "+----------------+-----------------+-----------------+--------------------------+-------------+-------------+\n"
              << "| Kode           | Stasiun Asal    | Stasiun Tujuan  | Nama Kereta              | Berangkat   | Tiba        |\n"
              << "+----------------+-----------------+-----------------+--------------------------+-------------+-------------+\n";

    for(const auto& j : filtered) {
        std::cout << "| " << std::left << std::setw(11) << j.kode << " | "
              << std::setw(15) << j.stasiunAsal << " | "
              << std::setw(15) << j.stasiunTujuan << " | "
              << std::setw(24) << j.namaKereta << " | "
              << std::setw(11) << j.waktuBerangkat << " | "
              << std::setw(11) << j.waktuTiba << " |\n";
    }
    std::cout << "+----------------+-----------------+-----------------+--------------------------+-------------+-------------+\n";
    std::cout << "\nTekan ENTER untuk melanjutkan...";
    std::cin.get();
}

void ManagementSystem::pesanTiket(const Pemesanan& pemesanan) {
    if(!isSeatAvailable(pemesanan.nomorKursi)) {
        throw std::runtime_error("Kursi sudah dipesan!");
    }
    bookingQueue.enqueue(pemesanan);
    confirmationStack.push(pemesanan);
    bookedSeats.insert(pemesanan.nomorKursi);
}

std::string ManagementSystem::generatePNR() const {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string pnr;
    for (int i = 0; i < 6; ++i) {
        pnr += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return pnr;
}

bool ManagementSystem::isSeatAvailable(const std::string& seat) const {
    return bookedSeats.find(seat) == bookedSeats.end();
}

void ManagementSystem::muatDariFile() {
    std::ifstream jadwalFile(JADWAL_FILE);
    std::ifstream pemesananFile(PEMESANAN_FILE);
    
    if (jadwalFile.is_open()) {
        Jadwal j;
        while (jadwalFile >> j.kode >> j.stasiunAsal >> j.stasiunTujuan 
               >> j.namaKereta >> j.tanggal >> j.waktuBerangkat >> j.waktuTiba) {
            jadwalKereta.push_back(j);
        }
    }
    
    if (pemesananFile.is_open()) {
        Pemesanan p;
        while (pemesananFile >> p.pnr >> p.namaPenumpang 
               >> p.nomorKursi >> p.kodeKereta >> p.confirmed) {
            if (p.confirmed) {
                bookedSeats.insert(p.nomorKursi);
            }
        }
    }
}

void ManagementSystem::simpanKeFile() const {
    std::ofstream jadwalFile(JADWAL_FILE);
    std::ofstream pemesananFile(PEMESANAN_FILE);
    
    for (const auto& j : jadwalKereta) {
        jadwalFile << j.kode << " " << j.stasiunAsal << " " << j.stasiunTujuan << " "
                  << j.namaKereta << " " << j.tanggal << " " << j.waktuBerangkat << " "
                  << j.waktuTiba << "\n";
    }
}

void ManagementSystem::sortSchedules() {
    std::sort(jadwalKereta.begin(), jadwalKereta.end(), 
        [](const Jadwal& a, const Jadwal& b) {
            if (a.tanggal != b.tanggal) {
                return a.tanggal < b.tanggal;
            }
            return a.waktuBerangkat < b.waktuBerangkat;
        });
}

void ManagementSystem::undoPerubahan() {
    if (!changeHistory.isEmpty()) {
        Jadwal lastChange = changeHistory.peek();
    // Cari dan hapus jadwal yang sesuai
    jadwalKereta.erase(
            std::remove_if(jadwalKereta.begin(), jadwalKereta.end(),
                [&lastChange](const Jadwal& j) {
                    return j.kode == lastChange.kode;
                }),
            jadwalKereta.end()
        );
        changeHistory.pop();
    }
}

void ManagementSystem::tambahJadwal(const Jadwal& jadwal) {
    jadwalKereta.push_back(jadwal);
    sortSchedules();
}

void ManagementSystem::prosesKonfirmasi() {
    while(!confirmationStack.isEmpty()) {
        Pemesanan p = confirmationStack.peek();
        std::cout << "\nKonfirmasi Data:\n"
                  << "PNR: " << p.pnr << "\n"
                  << "Nama: " << p.namaPenumpang << "\n"
                  << "Kursi: " << p.nomorKursi << "\n"
                  << "Konfirmasi (Y/N)? ";
        
        char input;
        std::cin >> input;
        if(toupper(input) == 'Y') {
    // Simpan data ke sistem
    confirmationStack.pop();
        } else {
    // Batalkan data pemesanan
    bookedSeats.erase(p.nomorKursi);
            confirmationStack.pop();
        }
    }
}
