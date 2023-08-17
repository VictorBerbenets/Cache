#include "compare.hpp"
#include <stdexcept>

namespace cmp = Caches_Comporator;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("invalid set of files\n");
    }
    
    std::string file_name = argv[1];
    std::ifstream test_file(file_name);

    std::size_t capacity{};
    test_file >> capacity;

    cmp::compare<std::string, std::size_t> comp(capacity, test_file);
    comp.dump( {argv[1]} );
}
