// Dmytro Demchuk K-25

#include <fstream>
#include <iostream>
#include <random>
#include <string>

class InputMaker {
    std::size_t read0_ = 0, write0_ = 0, read1_ = 0, write1_ = 0, text_ = 0;
    std::mt19937 random_;
    std::uniform_int_distribution<int> values_{1, 1000000};

    bool chooseType(const std::string& type) {
        if (type == "expected") { setCounts(10, 10, 50, 10, 20); return true; }
        if (type == "even") { setCounts(20, 20, 20, 20, 20); return true; }
        if (type == "mostly_string") { setCounts(1, 1, 1, 1, 96); return true; }
        return false;
    }

    void setCounts(int a, int b, int c, int d, int e) {
        read0_ = a; write0_ = b; read1_ = c;
        write1_ = d; text_ = e;
    }

    void scaleCounts(std::size_t total) {
        read0_ = read0_ * total / 100;
        write0_ = write0_ * total / 100;
        read1_ = read1_ * total / 100;
        write1_ = write1_ * total / 100;
        text_ = total - read0_ - write0_ - read1_ - write1_;
    }

    int pickOperation(std::size_t remaining) {
        std::uniform_int_distribution<std::size_t> pick(0, remaining - 1);
        std::size_t position = pick(random_);
        if (position < read0_) { --read0_; return 0; }
        position -= read0_;
        if (position < write0_) { --write0_; return 1; }
        position -= write0_;
        if (position < read1_) { --read1_; return 2; }
        position -= read1_;
        if (position < write1_) { --write1_; return 3; }
        --text_;
        return 4;
    }

    void writeOperation(std::ofstream& output, int operation) {
        if (operation == 0) output << "read 0\n";
        if (operation == 1) output << "write 0 " << values_(random_) << '\n';
        if (operation == 2) output << "read 1\n";
        if (operation == 3) output << "write 1 " << values_(random_) << '\n';
        if (operation == 4) output << "string\n";
    }

public:
    explicit InputMaker(unsigned seed) : random_(seed) {}

    bool create(const std::string& type, std::size_t count, const std::string& path) {
        if (!count || !chooseType(type)) return false;
        scaleCounts(count);
        std::ofstream output(path);
        if (!output) return false;
        for (std::size_t left = count; left > 0; --left)
            writeOperation(output, pickOperation(left));
        return static_cast<bool>(output);
    }
};

int main(int argc, char** argv) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <expected|even|mostly_string> <count> <seed> <output>\n";
        return 1;
    }
    std::size_t count;
    unsigned seed;
    try {
        std::size_t used = 0;
        count = std::stoull(argv[2], &used);
        if (used != std::string(argv[2]).size()) return 1;
        seed = std::stoul(argv[3], &used);
        if (used != std::string(argv[3]).size()) return 1;
    } catch (...) { return 1; }
    InputMaker maker(seed);
    if (!maker.create(argv[1], count, argv[4])) return 2;
    std::cout << "Created " << count << " operations in " << argv[4] << '\n';
    return 0;
}
