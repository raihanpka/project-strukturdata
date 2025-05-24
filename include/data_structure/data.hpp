#pragma once

#include <string>

struct Jadwalsss {
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

struct Stasiun {
    int id;
    std::string nama;
};

struct Kereta {
    std::string id;
    std::string nama;
    std::string kelas;
};

struct Rute {
    int id;
    int startId;
    int endId;
    int jarak;
};

struct Jadwal {
    int id;
    int idKereta;
    int idRute;
    std::string waktuBerangkat;
    std::string waktuTiba;
};