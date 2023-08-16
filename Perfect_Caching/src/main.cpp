#include <iostream>
#include "../include/perfect_cache.hpp"

int main() {
    namespace pfct = Perfect_Cache;
    
    pfct::size_type cache_capacity;
    std::cin >> cache_capacity;        
    pfct::cache<std::string, pfct::size_type> cache(cache_capacity);
    cache.give_data(std::cin);

    std::cout << cache.get_hits() << '\n';
}
