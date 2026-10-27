// Dmytro Demchuk K-25
#include "fields.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

struct Result {
    unsigned long long checksum = 0;
    unsigned long long operations = 0;
    bool ok = true;
};

bool apply(const std::string& command, std::istream& input, Fields& data, Result& result) {
    if (command == "string") {
        result.checksum += data.getText().size();
        return true;
    }
    int field;
    if (!(input >> field) || field < 0 || field > 1) return false;
    if (command == "read") {
        result.checksum += static_cast<unsigned int>(data.get(field));
        return true;
    }
    if (command == "write") {
        int value;
        if (!(input >> value)) return false;
        data.set(field, value);
        return true;
    }
    return false;
}

void runFile(const std::string& path, Result& result) {
    std::ifstream input(path);
    if (!input) { result.ok = false; return; }
    Fields data;
    std::string command;
    while (input >> command) {
        if (!apply(command, input, data, result)) {
            result.ok = false;
            return;
        }
        ++result.operations;
    }
    if (!input.eof()) result.ok = false;
}

int main(int argc, char** argv) {
    if (argc < 3 || argc > 5) {
        std::cerr << "Usage: run_tests <1|2|3> <input1> [input2] [input3]\n";
        return 1;
    }
    int count = 0;
    try {
        std::size_t used = 0;
        count = std::stoi(argv[1], &used);
        if (used != std::string(argv[1]).size()) return 1;
    } catch (...) { return 1; }
    if (count < 1 || count > 3 || argc != count + 2) return 1;

    Result a, b, c;
    const auto start = std::chrono::steady_clock::now();
    std::thread first(runFile, argv[2], std::ref(a));
    std::thread second, third;
    if (count > 1) second = std::thread(runFile, argv[3], std::ref(b));
    if (count > 2) third = std::thread(runFile, argv[4], std::ref(c));
    first.join();
    if (second.joinable()) second.join();
    if (third.joinable()) third.join();
    const auto end = std::chrono::steady_clock::now();

    double ms = std::chrono::duration<double, std::milli>(end - start).count();
    bool ok = a.ok && b.ok && c.ok;
    std::cout << std::fixed << std::setprecision(3)
              << "threads=" << count
              << " operations=" << a.operations + b.operations + c.operations
              << " time_ms=" << ms
              << " checksum=" << a.checksum + b.checksum + c.checksum
              << " status=" << (ok ? "OK" : "ERROR") << '\n';
    return ok ? 0 : 2;
}
