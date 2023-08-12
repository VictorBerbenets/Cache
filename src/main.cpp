#include <iostream>
#include "../LFU_Cache/cache.hpp"

int main() {
    namespace lfu = Cache_LFU;
    lfu::size_type number;
    lfu::size_type capacity;
    std::cin >> capacity >> number;

    lfu::cache<lfu::page_t, int> cch(capacity);
    std::cout << lfu::CheckHits(cch, number) << std::endl;
}
