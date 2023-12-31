#include "perfect_cache.hpp"

int main() {
    using size_type = yLAB::perfect_cache<std::string, std::size_t>::size_type;

    size_type cache_capacity;
    std::cin >> cache_capacity;
    if (!std::cin.good()) {
        throw std::runtime_error{"Cache capacity reading error!\n"};
    }
    yLAB::perfect_cache<std::string, size_type> cache(cache_capacity);
    cache.give_data(std::cin);

    std::cout << cache.get_hits() << '\n';
}

