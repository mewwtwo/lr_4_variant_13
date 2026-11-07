// Dmytro Demchuk K-25
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

namespace {

constexpr std::size_t kCount = 3000000;

struct Mix {
    int read0;
    int write0;
    int read1;
    int write1;
    int text;
};

struct Spec {
    const char* path;
    Mix mix;
};

const Spec kSpecs[] = {
    {"input/expected.txt", {10, 10, 50, 10, 20}},
    {"input/even.txt", {20, 20, 20, 20, 20}},
    {"input/mostly_string.txt", {1, 1, 1, 1, 96}},
};

std::vector<int> shuffledKinds(const Mix& mix, std::mt19937& random) {
    const int percents[] = {mix.read0, mix.write0, mix.read1, mix.write1};
    std::vector<int> kinds;
    kinds.reserve(kCount);
    for (int kind = 0; kind < 4; ++kind) kinds.insert(kinds.end(), kCount * percents[kind] / 100, kind);
    kinds.resize(kCount, 4);
    std::shuffle(kinds.begin(), kinds.end(), random);
    return kinds;
}

void writeOperation(std::ostream& out, int kind, int value) {
    switch (kind) {
        case 0: out << "read 0\n"; break;
        case 1: out << "write 0 " << value << '\n'; break;
        case 2: out << "read 1\n"; break;
        case 3: out << "write 1 " << value << '\n'; break;
        default: out << "string\n";
    }
}

bool writeFile(const Spec& spec, std::mt19937& random) {
    std::ofstream output(spec.path);
    if (!output) return false;
    std::uniform_int_distribution<int> values(1, 1000000);
    for (const int kind : shuffledKinds(spec.mix, random)) {
        writeOperation(output, kind, values(random));
    }
    return static_cast<bool>(output);
}

}

int main() {
    std::mt19937 random{std::random_device{}()};
    for (const Spec& spec : kSpecs) {
        if (!writeFile(spec, random)) {
            std::cerr << "Error: cannot write " << spec.path << " (run from the project folder)\n";
            return 1;
        }
        std::cout << "Created " << kCount << " operations in " << spec.path << '\n';
    }
    return 0;
}
