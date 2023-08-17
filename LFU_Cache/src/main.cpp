#include <iostream>
#include "cache.hpp"

namespace lfu = Cache_LFU;

template<typename T, typename KeyT = int>
std::size_t check_hits(lfu::cache<T, KeyT>& cch, std::size_t number) {
    std::size_t hits = 0;
    for (std::size_t count = 0; count < number; ++count) {
        lfu::page_t<T, KeyT> tmp_page {};
        std::cin >> tmp_page.page_id;
        hits += cch.lookup_update(tmp_page.page_id, tmp_page);
    }
    return hits;
}

int main() {
    using type = lfu::size_type;

    type number;
    type capacity;
    std::cin >> capacity >> number;
    
    lfu::cache<std::string, type> cch(capacity);
    std::cout << check_hits(cch, number) << std::endl;
}
