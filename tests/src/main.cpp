#include "test_generating.hpp"

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error{"Incorrect command line arguments! Expected number of tests\n"};
    }

    int tests_number{};
    tests_number = std::stoi(argv[1]);
    if (tests_number < 0) {
        throw std::invalid_argument{"Got negative number for number of tests\n"};
    }

    Tests::generator gen;
    gen.generate(tests_number);
}
