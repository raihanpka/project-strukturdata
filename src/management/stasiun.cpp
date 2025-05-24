#include "stasiun.hpp"

using namespace std;

void StasiunManager::tambahStasiun(const Stasiun& stasiun) {
    if (stasiun.nama.empty()) {
        throw invalid_argument("Semua data stasiun harus diisi.");
    }
    daftarStasiun.push_back(stasiun);
}
int StasiunManager::getId() const {
    return daftarStasiun[0].id;
}
string StasiunManager::getNama() const {
    return daftarStasiun[0].nama;
}