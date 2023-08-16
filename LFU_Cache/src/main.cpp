#include <iostream>
#include "cache_copy.hpp"

int main() {
    namespace lfu = Cache_LFU;
    using type = lfu::size_type;

    type number;
    type capacity;
    std::cin >> capacity >> number;
    
    lfu::cache<std::string, type> cch(capacity);
    std::cout << lfu::check_hits(cch, number) << std::endl;
}
