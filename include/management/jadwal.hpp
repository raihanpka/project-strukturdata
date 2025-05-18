#pragma once

#include "stack.hpp"
#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <vector>
#include <iostream>

class JadwalManager {
private:
    std::vector<Jadwal> daftarJadwal;

public:
    std::string generateKodeJadwal(
        const std::string& namaKereta,
        const std::string& stasiunAsal,
        const std::string& stasiunTujuan,
        const std::string& tanggal
    );
    void tambahJadwal(const Jadwal& jadwal);
    void tampilkanJadwal(const std::string& filterTanggal = "") const;
    void sortSchedules();
    void undoPerubahan();

    // Getter untuk akses baca saja (const)
    const std::vector<Jadwal>& getJadwal() const;

    // Getter untuk akses modifikasi (non-const)
    std::vector<Jadwal>& getDaftarJadwal();
};