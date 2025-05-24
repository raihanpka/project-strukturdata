#include "rute.hpp"

using namespace std;

 void RuteManager::tambahRute(const Rute& rute) {
    if (rute.startId <= 0 || rute.endId <= 0 || rute.jarak <= 0) {
        throw invalid_argument("Semua data rute harus diisi dan valid.");
    }
    daftarRute.push_back(rute);
}
int RuteManager::getId() const {
    return daftarRute[0].id;
}
int RuteManager::getStartId() const {
    return daftarRute[0].startId;
}
int RuteManager::getEndId() const {
    return daftarRute[0].endId;
}