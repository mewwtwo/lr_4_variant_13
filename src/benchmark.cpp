// Dmytro Demchuk K-25
#include "benchmark.hpp"

#include <chrono>
#include <thread>

#include "fields.hpp"

namespace {

using Iterator = std::vector<Operation>::const_iterator;

unsigned long long execute(Fields& fields, Iterator first, Iterator last) {
    unsigned long long checksum = 0;
    for (; first != last; ++first) {
        switch (first->kind) {
            case Kind::Read:
                checksum += static_cast<unsigned int>(fields.get(first->field));
                break;
            case Kind::Write:
                fields.set(first->field, first->value);
                break;
            case Kind::Text:
                checksum += static_cast<std::string>(fields).size();
                break;
        }
    }
    return checksum;
}

Iterator sliceBegin(const std::vector<Operation>& operations, int index, int threads) {
    return operations.begin() + static_cast<std::ptrdiff_t>(operations.size() * index / threads);
}

}

Run runOnce(const std::vector<Operation>& operations, int threads) {
    Fields fields;
    std::vector<unsigned long long> checksums(threads);
    std::vector<std::thread> pool;
    pool.reserve(threads);

    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < threads; ++i) {
        const auto first = sliceBegin(operations, i, threads);
        const auto last = sliceBegin(operations, i + 1, threads);
        pool.emplace_back([&fields, &checksums, i, first, last] {
            checksums[i] = execute(fields, first, last);
        });
    }
    for (auto& thread : pool) thread.join();
    const auto end = std::chrono::steady_clock::now();

    Run run{std::chrono::duration<double, std::milli>(end - start).count(), 0};
    for (const auto checksum : checksums) run.checksum += checksum;
    return run;
}
