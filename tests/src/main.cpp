#include "test_generating.hpp"

int main() {
    namespace test = Tests;
    std::size_t cap;
    std::size_t data_size;
    std::cin >> cap;
    std::cin >> data_size;
    
    test::weak_lfu lfu(cap);

    std::size_t hits{};
    std::size_t key{};
    for (std::size_t i = 0; i < data_size; ++i) {
        std::cin >> key;
        hits += lfu.lookup_update(key);
    }
    std::cout << hits << std::endl;
}
