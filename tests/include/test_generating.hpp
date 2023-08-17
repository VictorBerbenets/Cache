#include <iostream>
#include <list>
#include <ctime>
#include <string>
#include <fstream>

namespace Tests {
using type = std::size_t;

class weak_lfu {
    using Key = type;
    using cacheType = std::pair<Key, type>;
    using cacheIter = typename std::list<cacheType>::iterator;

    cacheIter find_minimum_freq(); //find key with minimum frequency
    bool is_full() const noexcept;
    cacheIter find(Key key);
public:
    weak_lfu(type capacity);
    weak_lfu() = default;

    bool lookup_update(Key key);
private:
    std::list<cacheType> cache_;
    type cache_size_;
    type capacity_;
};

weak_lfu::weak_lfu(type capacity):
        cache_size_{0}, capacity_(capacity) {}

bool weak_lfu::lookup_update(Key key) {
    auto cache_iter = find(key);
    if (cache_iter == cache_.end()) { // not found
        if (is_full()) {
            auto replace_iter    = find_minimum_freq();
            cache_.erase(replace_iter);
            cache_.push_back( {key, 1} );
        } else {
            cache_.push_back( {key, 1} );
            ++cache_size_;
        }
        return false;
    }
    cache_iter->second += 1;
    
    return true;
}

weak_lfu::cacheIter weak_lfu::find(Key key) {
    for (auto it = cache_.begin(); it != cache_.end(); ++it) {
        if (it->first == key) {
            return it;
        }
    }
    return cache_.end();//not found
}

bool weak_lfu::is_full() const noexcept {
    return cache_size_ == capacity_;
}

weak_lfu::cacheIter weak_lfu::find_minimum_freq() {
    cacheIter min_freq = cache_.begin();
    type min   = 4000000000; //for the first compare
    type count = 0;
    for (auto it = cache_.begin(); it != cache_.end() && count < cache_size_; ++it, ++count) {
        if (it->second < min) {
            min = it->second;
            min_freq = it;
            if (min == 1) {
                break;
            }
        }
    }
    return min_freq;
}

class lfu_generator {
    using u_int = std::size_t;
    
    const u_int MAX_CACHE_SIZE = 100;
    const u_int MAX_DATA_SIZE  = 1000000;
    const u_int MIN_DATA_SIZE  = 150;
    const u_int MAX_DATA_VALUE = 1000;
    const u_int MIN_DATA_VALUE = 1;

    const u_int MAX_TESTS_NUMBER = 150;
     
    void generate_test(u_int test_number);
public: 
    void generate(u_int tests_number);
};

void lfu_generator::generate(u_int test_number) {
    if (test_number > MAX_TESTS_NUMBER) {
        test_number = MAX_TESTS_NUMBER;
    }
    for (u_int count = 0; count < test_number; ++count) {
        std::cout << "count = " << count << '\n';
        generate_test(count);
    }
}

void lfu_generator::generate_test(u_int test_number) {
    std::srand(std::time(NULL));

    std::string test_file_name = "test" + std::to_string(test_number) + ".txt";
    std::ofstream test_file("../resources/" + test_file_name);

    u_int cache_cap = (std::rand() + 1) % MAX_CACHE_SIZE;
    u_int data_size = (std::rand() * std::rand() + MIN_DATA_SIZE) % MAX_DATA_SIZE;

    weak_lfu cache(cache_cap);

    test_file << cache_cap << ' ' << data_size;
    u_int hits = 0;
    for (u_int count = 0; count < data_size; ++count) {
        u_int key = (std::rand() + MIN_DATA_VALUE) % MAX_DATA_VALUE;
        hits += cache.lookup_update(key);
        test_file << ' ' <<  key;
    }
    
    std::string answ_name = "../resources/" + test_file_name + "_answ" + ".txt";
    std::ofstream answer(answ_name);
    answer << hits;
}

};
