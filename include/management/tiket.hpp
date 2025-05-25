#pragma once

#include "stack.hpp"
#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

class TiketManager {
private:
    Stack<Pemesanan> konfirmasiPemesanan;
    Queue<Pemesanan> antrianPemesanan;
    std::vector<Pemesanan> daftarPemesanan;
    std::unordered_map<std::string, std::unordered_set<std::string>> kursiTerpesan;

public:
    std::string generatePNR() const;
    bool isSeatAvailable(const std::string& kodeKereta, const std::string& seat) const;

    void pesanTiket(const Pemesanan& pemesanan);
    void prosesKonfirmasiPemesanan();

    void tampilkanTiketByPNR(const std::string& pnr) const;

    // Getters untuk akses baca saja (const)
    const std::vector<Pemesanan>& getDaftarPemesanan() const;
    const std::unordered_map<std::string, std::unordered_set<std::string>>& getKursiTerpesan() const;

    // Getters untuk akses modifikasi (non-const)
    std::vector<Pemesanan>& getMutableDaftarPemesanan();
    std::unordered_map<std::string, std::unordered_set<std::string>>& getMutableKursiTerpesan();
};
