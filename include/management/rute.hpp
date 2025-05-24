#pragma once

#include "stack.hpp"
#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class RuteManager {
private:
    vector<Rute> daftarRute;

public:
    void tambahRute(const Rute& rute);
    int getId() const;
    int getStartId() const;
    int getEndId() const;
};
