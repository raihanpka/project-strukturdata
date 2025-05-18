#pragma once

#include <string>

struct Jadwal {
    std::string kode;
    std::string stasiunAsal;
    std::string stasiunTujuan;
    std::string namaKereta;
    std::string tanggal;
    std::string waktuBerangkat;
    std::string waktuTiba;
};

struct Pemesanan {
    std::string pnr;
    std::string namaPenumpang;
    std::string nomorKursi;
    std::string kodeKereta;
    bool confirmed = false;
};
