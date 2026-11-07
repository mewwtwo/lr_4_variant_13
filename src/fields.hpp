// Dmytro Demchuk K-25
#pragma once

#include <mutex>
#include <string>

class Fields {
public:
    static constexpr int kCount = 2;

    int get(int field) const;
    void set(int field, int value);
    operator std::string() const;

private:
    struct alignas(64) Slot {
        int value = 0;
        mutable std::mutex lock;
    };

    Slot slots_[kCount];
};
