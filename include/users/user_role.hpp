#pragma once

#include <string>

enum class UserRole {
    ADMIN,
    PASSENGER,
    NONE
};

class UserContext {
private:
    UserRole role;
    std::string username;

public:
    UserContext() : role(UserRole::NONE), username("Guest") {}

    bool authenticateAdmin(const std::string& username, const std::string& password);
    void setPassenger();

    UserRole getRole() const;
    std::string getUsername() const;
};
