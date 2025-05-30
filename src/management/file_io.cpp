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
    while (!jadwalList.empty()) {
        jadwalList.pop_back();
    }
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
    while (!tiketList.empty()) {
        tiketList.pop_back();
    }
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

void FileIO::simpanantrean(const Queue<Pemesanan>& antreanList, const string& path) {
    ofstream out(path);
    out << "pnr,namaPenumpang,nomorKursi,kodeJadwal,confirmed\n";
    Queue<Pemesanan> tempQueue = antreanList;
    while (!tempQueue.isEmpty()) {
        Pemesanan t = tempQueue.peek();
        out << t.pnr << "," << t.namaPenumpang << "," << t.nomorKursi << ","
            << t.kodeJadwal << "," << (t.confirmed ? "1" : "0") << "\n";
        tempQueue.dequeue();
    }
}

void FileIO::muatantrean(Queue<Pemesanan>& antreanList, const string& path) {
    while (!antreanList.isEmpty()) {
        antreanList.dequeue();
    }
    ifstream in(path);
    string line;
    getline(in, line); 
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Pemesanan p;
        string confirmedStr;
        if (!getline(ss, p.pnr, ',') ||
            !getline(ss, p.namaPenumpang, ',') ||
            !getline(ss, p.nomorKursi, ',') ||
            !getline(ss, p.kodeJadwal, ',')) {
            continue;
        }
        antreanList.enqueue(p);
    }
}