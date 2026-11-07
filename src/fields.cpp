// Dmytro Demchuk K-25
#include "fields.hpp"

#include <stdexcept>

namespace {

void checkField(int field) {
    if (field < 0 || field >= Fields::kCount) throw std::out_of_range("field number");
}

}

int Fields::get(int field) const {
    checkField(field);
    std::lock_guard<std::mutex> guard(slots_[field].lock);
    return slots_[field].value;
}

void Fields::set(int field, int value) {
    checkField(field);
    std::lock_guard<std::mutex> guard(slots_[field].lock);
    slots_[field].value = value;
}

Fields::operator std::string() const {
    int first = 0;
    int second = 0;
    {
        std::scoped_lock guard(slots_[0].lock, slots_[1].lock);
        first = slots_[0].value;
        second = slots_[1].value;
    }
    return std::to_string(first) + " " + std::to_string(second);
}
