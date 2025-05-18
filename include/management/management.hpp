#pragma once

#include "jadwal.hpp"
#include "tiket.hpp"
#include "file_io.hpp"
#include "user_role.hpp"

#include <iostream>
#include <string>

class ManagementSystem {
private:
    JadwalManager jadwalManager;
    TiketManager tiketManager;
    UserContext user;

public:
    void muatDariFile();
    void simpanKeFile() const;

    UserContext& getUser();
    JadwalManager& getJadwalManager();
    TiketManager& getTiketManager();
};