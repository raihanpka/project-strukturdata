#include "file_io.hpp"
#include <fstream>
#include <sstream>

using namespace std;

void FileIO::simpanJadwal(const vector<Jadwal>& jadwalList, const string& path) {
    ofstream out(path);
    out << "kodeJadwal,stasiunAsal,stasiunTujuan,namaKereta,tanggal,waktuBerangkat,waktuTiba\n";
    for (const auto& j : jadwalList) {
        out << j.kode << "," << j.stasiunAsal << "," << j.stasiunTujuan << ","
            << j.namaKereta << "," << j.tanggal << ","
            << j.waktuBerangkat << "," << j.waktuTiba << "\n";
    }
}

void FileIO::muatJadwal(vector<Jadwal>& jadwalList, const string& path) {
    ifstream in(path);
    string line;
    getline(in, line);
    while (getline(in, line)) {
        stringstream ss(line);
        Jadwal j;
        getline(ss, j.kode, ',');
        getline(ss, j.stasiunAsal, ',');
        getline(ss, j.stasiunTujuan, ',');
        getline(ss, j.namaKereta, ',');
        getline(ss, j.tanggal, ',');
        getline(ss, j.waktuBerangkat, ',');
        getline(ss, j.waktuTiba, ',');
        jadwalList.push_back(j);
    }
}

void FileIO::simpanTiket(const vector<Pemesanan>& tiketList, const string& path) {
    ofstream out(path);
    out << "pnr,namaPenumpang,nomorKursi,kodeJadwal,confirmed\n";
    for (const auto& t : tiketList) {
        out << t.pnr << "," << t.namaPenumpang << "," << t.nomorKursi << ","
            << t.kodeJadwal << "," << t.confirmed << "\n";
    }
}

void FileIO::muatTiket(vector<Pemesanan>& tiketList,
                       unordered_map<string, unordered_set<string>>& kursiMap,
                       const string& path) {
    ifstream in(path);
    string line;
    getline(in, line);
    while (getline(in, line)) {
        stringstream ss(line);
        Pemesanan p;
        string confirmedStr;
        getline(ss, p.pnr, ',');
        getline(ss, p.namaPenumpang, ',');
        getline(ss, p.nomorKursi, ',');
        getline(ss, p.kodeJadwal, ',');
        getline(ss, confirmedStr, ',');
        p.confirmed = (confirmedStr == "1" || confirmedStr == "true");
        if (p.confirmed) {
            tiketList.push_back(p);
            kursiMap[p.kodeJadwal].insert(p.nomorKursi);
        }
    }
}

void FileIO::simpanAntrian(const Queue<Pemesanan>& antrianList, const string& path) {
    ofstream out(path);
    out << "pnr,namaPenumpang,nomorKursi,kodeJadwal,confirmed\n";
    Queue<Pemesanan> tempQueue = antrianList;
    while (!tempQueue.isEmpty()) {
        Pemesanan t = tempQueue.peek();
        out << t.pnr << "," << t.namaPenumpang << "," << t.nomorKursi << ","
            << t.kodeJadwal << "," << t.confirmed << "\n";
        tempQueue.dequeue();
    }
}
void FileIO::muatAntrian(Queue<Pemesanan>& antrianList, const string& path) {
    ifstream in(path);
    string line;
    getline(in, line);
    while (getline(in, line)) {
        stringstream ss(line);
        Pemesanan p;
        string confirmedStr;
        getline(ss, p.pnr, ',');
        getline(ss, p.namaPenumpang, ',');
        getline(ss, p.nomorKursi, ',');
        getline(ss, p.kodeJadwal, ',');
        getline(ss, confirmedStr, ',');
        p.confirmed = (confirmedStr == "0" || confirmedStr == "false");
        antrianList.enqueue(p);
    }
}