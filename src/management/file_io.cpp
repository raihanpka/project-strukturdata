#include "file_io.hpp"
#include <fstream>
#include <sstream>

using namespace std;

void FileIO::simpanJadwal(const vector<Jadwal>& jadwalList, const string& path) {
    ofstream out(path);
    for (const auto& j : jadwalList) {
        out << j.kode << "," << j.stasiunAsal << "," << j.stasiunTujuan << ","
            << j.namaKereta << "," << j.tanggal << ","
            << j.waktuBerangkat << "," << j.waktuTiba << "\n";
    }
}

void FileIO::muatJadwal(vector<Jadwal>& jadwalList, const string& path) {
    ifstream in(path);
    Jadwal j;
    while (in >> j.kode >> j.stasiunAsal >> j.stasiunTujuan
              >> j.namaKereta >> j.tanggal >> j.waktuBerangkat >> j.waktuTiba) {
        jadwalList.push_back(j);
    }
}

void FileIO::simpanTiket(const vector<Pemesanan>& tiketList, const string& path) {
    ofstream out(path);
    for (const auto& t : tiketList) {
        out << t.pnr << "," << t.namaPenumpang << "," << t.nomorKursi << ","
            << t.kodeKereta << "," << t.confirmed << "\n";
    }
}

void FileIO::muatTiket(vector<Pemesanan>& tiketList,
                       unordered_map<string, unordered_set<string>>& kursiMap,
                       const string& path) {
    ifstream in(path);
    Pemesanan p;
    while (in >> p.pnr >> p.namaPenumpang >> p.nomorKursi >> p.kodeKereta >> p.confirmed) {
        if (p.confirmed) {
            tiketList.push_back(p);
            kursiMap[p.kodeKereta].insert(p.nomorKursi);
        }
    }
}
