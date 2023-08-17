#include <iostream>
#include <vector>
#include <list>
#include <ctime>
#include <string>
#include <fstream>

#define __LFU_TEST_GENERATING__
#define __PERFECT_TEST_GENERATING__

namespace Tests {
using type = std::size_t;

//------------------------------------------------------------------------------------------//
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
//------------------------------------------------------------------------------------------//

class weak_perfect {
    using u_int     = std::size_t;
    using Key       = u_int;
    using cacheIter = typename std::list<Key>::iterator;
    
    cacheIter find(Key key);
    bool is_full() const noexcept;
    cacheIter find_farthest_value();
    void lookup_update();
public:
template<typename Iter>
    weak_perfect(u_int capacity, Iter begin, Iter end);
    
    u_int get_hits() const noexcept;
private:
    u_int capacity_;
    u_int cache_size_;
    std::list<Key> buffer_;
    std::list<Key> cache_;

    u_int hits_;
};

template<typename Iter>
weak_perfect::weak_perfect(u_int capacity, Iter begin, Iter end): 
               capacity_{capacity}, buffer_(begin, end) {
    lookup_update();
}

void weak_perfect::lookup_update() {
    for (auto buff_it : buffer_) {
        auto pos_it = find(buff_it);
        if (pos_it == cache_.end()) {
            if (is_full()) {
                auto most_far = find_farthest_value();
                *most_far = buff_it; 
            } else {
                cache_.push_back(buff_it);
                ++cache_size_;
            }
        } else {
            ++hits_;
        }

        buffer_.pop_front();
    }
}

bool weak_perfect::is_full() const noexcept {
    return cache_size_ == capacity_;
}

weak_perfect::cacheIter weak_perfect::find_farthest_value() {
    auto most_far = buffer_.begin();
    u_int distance = 0;
    for (cacheIter it1 = cache_.begin(); it1 != cache_.end(); ++it1) {
        u_int offset = 0;
        for (cacheIter it2 = buffer_.begin(); it2 != buffer_.end(); ++it2, ++offset) {
            if (*it1 == *it2 && offset > distance) {
                distance = offset;
                most_far = it1;
            }
        }
    }   
    return most_far;
}

weak_perfect::cacheIter weak_perfect::find(Key key) {
    for (auto iter = cache_.begin(); iter != cache_.end(); ++iter) {
        if (*iter == key) {
            return iter;
        }
    }
    return cache_.end();
}

weak_perfect::u_int weak_perfect::get_hits() const noexcept {
    return hits_;
}
//------------------------------------------------------------------------------------------//

class generator {
    using u_int = std::size_t;
    
    const u_int MAX_CACHE_SIZE = 100;
    const u_int MAX_DATA_SIZE  = 1000000;
    const u_int MIN_DATA_SIZE  = 150;
    const u_int MAX_DATA_VALUE = 1000;
    const u_int MIN_DATA_VALUE = 1;

    const u_int MAX_TESTS_NUMBER = 150;
     
    void generate_lfu_files(u_int test_number);
    void generate_perfect_files(u_int test_number);
public: 
    void generate(u_int tests_number);
};

void generator::generate(u_int test_number) {
    std::srand(std::time(NULL));
    
    if (test_number > MAX_TESTS_NUMBER) {
        test_number = MAX_TESTS_NUMBER;
    }
    for (u_int count = 0; count < test_number; ++count) {
        std::cout << "count = " << count << '\n';
#ifdef __LFU_TEST_GENERATING__
//        std::cout << "DEFINED LFU\n";
        generate_lfu_files(u_int #count);
#endif
#ifdef __PERFECT_TEST_GENERATING__
//        std::cout << "DEFINED PERFECT\n";
        generate_perfect_files(u_int count);
#endif
    }
}

void generator::generate_lfu_files(u_int test_number) {
    std::string test_file_name = "test" + std::to_string(test_number) + ".txt";
    std::ofstream test_file("../lfu_resources/" + test_file_name);

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
    
    std::string answ_name = "../lfu_resources/" + test_file_name + "_answ" + ".txt";
    std::ofstream answer(answ_name);
    answer << hits;
}

void generator::generate_perfect_files(u_int test_number) {
    std::string test_file_name = "test" + std::to_string(test_number);
    std::ofstream test_file("../perfect_resources/" + test_file_name + ".txt");

    u_int cache_cap = (std::rand() + 1) % MAX_CACHE_SIZE;
    u_int data_size = (std::rand() * std::rand() + MIN_DATA_SIZE) % MAX_DATA_SIZE;
    
    std::vector<u_int> data{};
    data.reserve(data_size);

    test_file << cache_cap << ' ' << data_size;
    for (u_int count = 0; count < data_size; ++count) {
        u_int key = (std::rand() + MIN_DATA_VALUE) % MAX_DATA_VALUE;
        data.push_back(key);
        test_file << ' ' <<  key;
    }

    weak_perfect cache(cache_cap, data.begin(), data.end());
   
    std::string answ_name = "../perfect_resources/" + test_file_name + "_answ" + ".txt";
    std::ofstream answer(answ_name);
    answer << cache.get_hits();

}
//------------------------------------------------------------------------------------------//

};
