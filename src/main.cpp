// Dmytro Demchuk K-25
#include <array>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "benchmark.hpp"
#include "operations.hpp"

namespace {

constexpr int kRuns = 10;
constexpr int kMaxThreads = 3;
const char* const kFiles[] = {"input/expected.txt", "input/even.txt", "input/mostly_string.txt"};

struct FileResult {
    std::string name;
    std::array<double, kMaxThreads> averageMs{};
};

double measure(const std::vector<Operation>& operations, int threads) {
    double total = 0;
    Run run{};
    for (int i = 0; i < kRuns; ++i) {
        run = runOnce(operations, threads);
        total += run.milliseconds;
    }
    std::cout << "threads=" << threads << " operations=" << operations.size()
              << " runs=" << kRuns << " avg_ms=" << total / kRuns
              << " checksum=" << run.checksum << '\n';
    return total / kRuns;
}

FileResult measureFile(const std::string& path) {
    const std::vector<Operation> operations = loadOperations(path);
    FileResult result{std::filesystem::path(path).filename().string(), {}};
    std::cout << result.name << '\n';
    for (int threads = 1; threads <= kMaxThreads; ++threads) {
        result.averageMs[threads - 1] = measure(operations, threads);
    }
    return result;
}

void printTable(const std::vector<FileResult>& results) {
    std::cout << "\nAverage time, ms (" << kRuns << " runs)\n"
              << std::left << std::setw(28) << "file";
    for (int threads = 1; threads <= kMaxThreads; ++threads) {
        std::cout << std::right << std::setw(12) << (std::to_string(threads) + " thr.");
    }
    std::cout << '\n' << std::fixed << std::setprecision(2);
    for (const auto& result : results) {
        std::cout << std::left << std::setw(28) << result.name;
        for (const double ms : result.averageMs) std::cout << std::right << std::setw(12) << ms;
        std::cout << '\n';
    }
}

}

int main() {
#ifndef NDEBUG
    std::cerr << "Warning: debug build, the measured time is not reliable\n";
#endif
    try {
        std::vector<FileResult> results;
        for (const char* path : kFiles) results.push_back(measureFile(path));
        printTable(results);
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
