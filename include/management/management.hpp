// Header untuk sistem manajemen kereta api
#pragma once
#include <string>

#include "../data_structure/stack.hpp"
#include "../data_structure/queue.hpp"
#include "../data.hpp"
#include <vector>
#include <unordered_set>

class ManagementSystem {
public:
    enum class UserRole { ADMIN, PASSENGER };
    
private:
    UserRole currentUserRole;
    std::string currentUsername;
    
    Stack<Jadwal> changeHistory;
    Queue<Pemesanan> bookingQueue;
    Stack<Pemesanan> confirmationStack;
    std::vector<Jadwal> jadwalKereta;
    std::unordered_set<std::string> bookedSeats;
    
    void sortSchedules();
    bool isSeatAvailable(const std::string& nomorKursi) const;
    
public:
    void tambahJadwal(const Jadwal& jadwal);
    void tampilkanJadwal(const std::string& filterTanggal = "") const;
    void undoPerubahan();
    void pesanTiket(const Pemesanan& pemesanan);
    void prosesKonfirmasi();
    void simpanKeFile() const;
    void muatDariFile();
    // Metode autentikasi
    bool authenticateAdmin(const std::string& username, const std::string& password);
    void setPassengerMode();
    
    // Metode kontrol akses
    bool isAdmin() const { return currentUserRole == UserRole::ADMIN; }
    bool isPassenger() const { return currentUserRole == UserRole::PASSENGER; }
    std::string getCurrentUsername() const { return currentUsername; }
    
    std::string generateKodeJadwal(const std::string& namaKereta, const std::string& stasiunAsal, const std::string& stasiunTujuan, const std::string& tanggal);
    std::string generatePNR() const;
};
