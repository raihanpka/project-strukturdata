// File Utama Program Sistem Kereta Api

// Cara menjalankan:
// g++ -std=c++17 -Iinclude src/*.cpp src/data_structure/*.cpp src/management/*.cpp -o SISTEMKAI
// ./SISTEMKAI

#include "../include/management/management.hpp"
#include "../include/file_path.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>

void adminMenu(ManagementSystem& sys) {
    int choice;
    do {
        std::cout << "\nMenu Admin:\n"
                  << "1. Tambah Jadwal Kereta\n"
                  << "2. Lihat Semua Pemesanan\n"
                  << "3. Kembali\n"
                  << "Pilih: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                Jadwal j;
                std::cout << "Stasiun Asal: ";
                std::cin.ignore();
                std::getline(std::cin, j.stasiunAsal);
                std::cout << "Stasiun Tujuan: ";
                std::getline(std::cin, j.stasiunTujuan);
                std::cout << "Nama Kereta: ";
                std::getline(std::cin, j.namaKereta);
                std::cout << "Tanggal (DD-MM-YYYY): ";
                std::getline(std::cin, j.tanggal);
                std::cout << "Waktu Berangkat: ";
                std::getline(std::cin, j.waktuBerangkat);
                std::cout << "Waktu Tiba: ";
                std::getline(std::cin, j.waktuTiba);
                
                j.kode = sys.generateKodeJadwal(j.namaKereta, j.stasiunAsal, j.stasiunTujuan, j.tanggal);
                std::cout << "Kode Kereta yang digenerate: " << j.kode << "\n";
                
                sys.tambahJadwal(j);
                break;
            }
            case 2: {
                std::string tanggal;
                do {
                    std::cout << "Masukkan tanggal (DD-MM-YYYY): ";
                    std::cin.ignore();
                    std::getline(std::cin, tanggal);
                    
                // Memvalidasi format tanggal (DD-MM-YYYY)
                    std::tm tm = {};
                    std::istringstream ss(tanggal);
                    ss >> std::get_time(&tm, "%d-%m-%Y");
                    if (ss.fail()) {
                        std::cout << "Format tanggal tidak valid. Gunakan format DD-MM-YYYY\n";
                        continue;
                    }
                    break;
                } while(true);
                sys.tampilkanJadwal(tanggal);
                break;
            }
            default:
                std::cout << "Pilihan tidak valid!\n";
                break;
        }
    } while(choice != 3);
}

void userMenu(ManagementSystem& sys) {
    int choice;
    do {
        std::cout << "\nMenu Penumpang:\n"
                  << "1. Lihat Jadwal\n"
                  << "2. Pesan Tiket\n"
                  << "3. Kembali\n"
                  << "Pilih: ";
        std::cin >> choice;
        
        switch(choice) {
            case 1: {
                std::string tanggal;
                do {
                    std::cout << "Masukkan tanggal (DD-MM-YYYY): ";
                    std::cin.ignore();
                    std::getline(std::cin, tanggal);
                    
                    // Memvalidasi format tanggal (DD-MM-YYYY)
                    std::tm tm = {};
                    std::istringstream ss(tanggal);
                    ss >> std::get_time(&tm, "%d-%m-%Y");
                    if (ss.fail()) {
                        std::cout << "Format tanggal tidak valid. Gunakan format DD-MM-YYYY\n";
                        continue;
                    }
                    break;
                } while(true);
                sys.tampilkanJadwal(tanggal);
                break;
            }
            case 2: {
                Pemesanan p;
                std::cout << "Nama Penumpang: ";
                std::cin.ignore();
                std::getline(std::cin, p.namaPenumpang);
                std::cout << "Nomor Kursi: ";
                std::getline(std::cin, p.nomorKursi);
                std::cout << "Kode Kereta: ";
                std::getline(std::cin, p.kodeKereta);
                p.pnr = sys.generatePNR();
                
                try {
                    sys.pesanTiket(p);
                    sys.prosesKonfirmasi();
                } catch(const std::exception& e) {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            default:
                std::cout << "Pilihan tidak valid!\n";
                break;
        }
    } while(choice != 3);
}

int main() {
    ManagementSystem system;
    system.muatDariFile();
    
    int mainChoice;
    do {
        std::cout << "\nSistem Penjadwalan Kereta Api\n"
                  << "1. Admin\n"
                  << "2. Penumpang\n"
                  << "3. Keluar\n"
                  << "Pilih: ";
        std::cin >> mainChoice;
        
        switch(mainChoice) {
            case 1: {
                std::string username, password;
                std::cout << "Username Admin: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;
                
                if (system.authenticateAdmin(username, password)) {
                    adminMenu(system);
                } else {
                    std::cout << "Autentikasi gagal!\n";
                }
                break;
            }
            case 2:
                userMenu(system);
                break;
        }
    } while(mainChoice != 3);
    
    system.simpanKeFile();
    return 0;
}