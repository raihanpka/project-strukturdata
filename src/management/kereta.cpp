#include "kereta.hpp"

using namespace std;

void KeretaManager::tambahKereta(const Kereta& kereta) {
    if (kereta.id.empty() || kereta.nama.empty() || kereta.kelas.empty()) {
        throw invalid_argument("Semua data kereta harus diisi dan valid.");
    }
    daftarKereta.push_back(kereta);
}

string KeretaManager::getId() const {
    return daftarKereta[0].id;
}
string KeretaManager::getNama() const {
    return daftarKereta[0].nama;
}
string KeretaManager::getKelas() const {
    return daftarKereta[0].kelas;
}