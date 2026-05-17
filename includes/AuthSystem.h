#ifndef AUTH_SYSTEM_H
#define AUTH_SYSTEM_H

#include <string>

enum class UserRole {
    NONE,
    ADMIN,
    MANAGER,
    CUSTOMER
};

class AuthSystem {
public:
    static UserRole stringToRole(const std::string& roleStr) {
        if (roleStr == "admin") return UserRole::ADMIN;
        if (roleStr == "manager") return UserRole::MANAGER;
        if (roleStr == "customer") return UserRole::CUSTOMER;
        return UserRole::NONE;
    }
};

#endif // AUTH_SYSTEM_H