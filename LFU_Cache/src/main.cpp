#include <iostream>
#include "cache.hpp"


template<typename T, typename KeyT = int>
std::size_t check_hits(yLAB::cache<T, KeyT>& cch, std::size_t number) {
    std::size_t hits = 0;
    for (std::size_t count = 0; count < number; ++count) {
        yLAB::page_t<T, KeyT> tmp_page {};
        std::cin >> tmp_page.page_id;
        hits += cch.lookup_update(tmp_page.page_id, tmp_page);
    }
    return hits;
}

int main() {
    using size_type = yLAB::size_type;

    size_type number;
    size_type capacity;
    std::cin >> capacity >> number;
    
    yLAB::cache<std::string, size_type> cch(capacity);
    std::cout << check_hits(cch, number) << std::endl;
}
