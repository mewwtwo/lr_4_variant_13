// Dmytro Demchuk K-25
#include "operations.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include "fields.hpp"

namespace {

Operation parseLine(const std::string& line) {
    std::istringstream input(line);
    std::string command;
    input >> command;
    Operation operation{Kind::Text, 0, 0};
    if (command == "string") {
        operation.kind = Kind::Text;
    } else if (command == "read" || command == "write") {
        operation.kind = command == "read" ? Kind::Read : Kind::Write;
        if (!(input >> operation.field)) throw std::runtime_error("missing field number");
        if (operation.field < 0 || operation.field >= Fields::kCount) {
            throw std::runtime_error("field number out of range");
        }
        if (operation.kind == Kind::Write && !(input >> operation.value)) {
            throw std::runtime_error("missing value");
        }
    } else {
        throw std::runtime_error("unknown command \"" + command + "\"");
    }
    std::string extra;
    if (input >> extra) throw std::runtime_error("unexpected \"" + extra + "\"");
    return operation;
}

}

std::vector<Operation> loadOperations(const std::string& path) {
    std::ifstream input(path);
    if (!input) throw std::runtime_error(path + ": cannot open file");
    std::vector<Operation> operations;
    std::string line;
    for (int number = 1; std::getline(input, line); ++number) {
        if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
        try {
            operations.push_back(parseLine(line));
        } catch (const std::runtime_error& error) {
            throw std::runtime_error(path + ":" + std::to_string(number) + ": " + error.what());
        }
    }
    if (operations.empty()) throw std::runtime_error(path + ": no operations");
    return operations;
}
