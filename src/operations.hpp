// Dmytro Demchuk K-25
#pragma once

#include <string>
#include <vector>

enum class Kind { Read, Write, Text };

struct Operation {
    Kind kind;
    int field;
    int value;
};

std::vector<Operation> loadOperations(const std::string& path);
