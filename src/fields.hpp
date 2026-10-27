// Dmytro Demchuk K-25
#pragma once
#include <mutex>
#include <string>

class Fields {
    int values_[2] = {0, 0};
    mutable std::mutex first_;
    mutable std::mutex second_;
public:
    int get(int field) const;
    void set(int field, int value);
    std::string getText() const;
};
