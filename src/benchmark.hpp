// Dmytro Demchuk K-25
#pragma once

#include <vector>

#include "operations.hpp"

struct Run {
    double milliseconds;
    unsigned long long checksum;
};

Run runOnce(const std::vector<Operation>& operations, int threads);
