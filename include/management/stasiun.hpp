#pragma once

#include "stack.hpp"
#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class StasiunManager {
private:
    vector<Stasiun> daftarStasiun;
public:
    void tambahStasiun(const Stasiun& stasiun);
    int getId() const;
    string getNama() const;
};