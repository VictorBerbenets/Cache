#include <iostream>
#include "../include/cache.hpp"

int main() {
    namespace lfu = Cache_LFU;
    lfu::size_type number;
    lfu::size_type capacity;
    std::cin >> capacity >> number;

    lfu::cache<lfu::page_t, int> cch(capacity);
    std::cout << lfu::check_hits(cch, number) << std::endl;
}
