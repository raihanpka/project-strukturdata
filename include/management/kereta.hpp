#pragma once

#include "stack.hpp"
#include "queue.hpp"
#include "data.hpp"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class KeretaManager {
private:
    vector<Kereta> daftarKereta;

public:
    void tambahKereta(const Kereta& kereta);
    string getId() const;
    string getNama() const;
    string getKelas() const;
};
