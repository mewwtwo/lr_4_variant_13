// Dmytro Demchuk K-25
#include "fields.hpp"
#include <stdexcept>

int Fields::get(int field) const {
    if (field == 0) {
        std::lock_guard<std::mutex> lock(first_);
        return values_[0];
    }
    if (field == 1) {
        std::lock_guard<std::mutex> lock(second_);
        return values_[1];
    }
    throw std::out_of_range("field");
}

void Fields::set(int field, int value) {
    if (field == 0) {
        std::lock_guard<std::mutex> lock(first_);
        values_[0] = value;
        return;
    }
    if (field == 1) {
        std::lock_guard<std::mutex> lock(second_);
        values_[1] = value;
        return;
    }
    throw std::out_of_range("field");
}

std::string Fields::getText() const {
    std::scoped_lock lock(first_, second_);
    return std::to_string(values_[0]) + " " + std::to_string(values_[1]);
}
