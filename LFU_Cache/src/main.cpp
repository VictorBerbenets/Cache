#include <iostream>
#include "cache.hpp"

int main() {
    namespace lfu = Cache_LFU;
    using type = lfu::size_type;

    type number;
    type capacity;
    std::cin >> capacity >> number;
    
    lfu::cache<std::string, type> cch(capacity);
    std::cout << lfu::check_hits(cch, number) << std::endl;
}
