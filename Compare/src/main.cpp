#include "compare.hpp"


int main() {
    namespace cmp = Caches_Comporator;
    
    std::size_t capacity{};
    std::cin >> capacity;

    cmp::compare<std::string, std::size_t> comp(capacity, std::cin);
    comp.dump();
}
