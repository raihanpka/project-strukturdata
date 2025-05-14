#ifndef DATA_HPP
#define DATA_HPP

#include <string>

struct Jadwal {
    std::string kode;
    std::string stasiunAsal;
    std::string stasiunTujuan;
    std::string namaKereta;
    std::string tanggal;
    std::string waktuBerangkat;
    std::string waktuTiba;
    std::string hari;  // Untuk pengelompokan
};

struct Pemesanan {
    std::string pnr;
    std::string namaPenumpang;
    std::string nomorKursi;
    std::string kodeKereta;
    std::string kodeJadwal;  // Relasi ke jadwal
    bool confirmed;
    std::string tanggalPemesanan;
};

#endif