#include <iostream>
#include "lfu_cache.hpp"


template<typename T, typename KeyT = int>
std::size_t check_hits(yLAB::lfu_cache<T, KeyT>& cch, std::size_t number) {
    std::size_t hits = 0;
    using page_t = typename yLAB::lfu_cache<T, KeyT>::page_t; 
    auto get_page = [](KeyT key) -> page_t {
                        page_t page;
                        page.second = key;
                        return page;
                        };

    for (std::size_t count = 0; count < number; ++count) {
        std::size_t key{};
        std::cin >> key;
        if (!std::cin.good()) {
            throw std::runtime_error{"Reading data error!\n"};
        }
        hits += cch.lookup_update(key, get_page);
    }
    return hits;
}

int main() {
    using size_type = typename yLAB::lfu_cache<std::string, std::size_t>::size_type;

    size_type capacity;
    std::cin >> capacity;
    if (!std::cin.good()) {
        throw std::runtime_error{"Cache capacity reading error!\n"};
    }
    
    size_type number;
    std::cin >> number;
    if (!std::cin.good()) {
        throw std::runtime_error{"Cache data size reading error!\n"};
    }

    yLAB::lfu_cache<std::string, size_type> cch(capacity);
    std::cout << check_hits(cch, number) << std::endl;
}
