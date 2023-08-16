#include "compare.hpp"


int main() {
    namespace cmp = Caches_Comporator;
    
    std::size_t capacity{};
    std::cin >> capacity;

    cmp::compare<std::string, std::size_t> comp(capacity, std::cin);

    std::cout << "lfu_time: " << comp.get_lfu_time() << "\t\tlfu_hits: " <<
        comp.get_lfu_hits() << '\n';
    std::cout << "perfect_time: " << comp.get_perfect_time() << "\t\tperfect_hits: " <<
        comp.get_perfect_hits() << '\n';
}
