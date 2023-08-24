#include "compare.hpp"

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("invalid set of files\n");
    }

    std::string file_name = argv[1];
    std::ifstream test_file(file_name);
    std::size_t capacity{};
    test_file >> capacity;
    if (!test_file.good()) {
        std::runtime_error{"Invalid capacity for cache\n"};
    }
    yLAB::compare<std::string, std::size_t> comp(capacity, test_file);
    comp.dump(file_name);
}

