#pragma once

#include "stack.hpp"
#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class JadwalManager {
private:
    Stack<Jadwalsss> konfirmasiJadwal;  // Stack untuk menyimpan jadwal yang dihapus
    vector<Jadwalsss> daftarJadwal;

public:
    bool isValidTanggal(const string& tanggal);
    bool isValidWaktu(const string& waktu);
    string generateKodeJadwal(
        const string& namaKereta,
        const string& stasiunAsal,
        const string& stasiunTujuan,
        const string& tanggal
    );
    void tambahJadwal(const Jadwalsss& jadwal);
    void tampilkanJadwal(const string& filterTanggal = "") const;
    void sortSchedules();
    void prosesKonfirmasiJadwal();

    // Getter untuk akses baca saja (const)
    const vector<Jadwalsss>& getJadwal() const;

    // Getter untuk akses modifikasi (non-const)
    vector<Jadwalsss>& getDaftarJadwal();
};