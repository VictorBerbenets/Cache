#include <iostream>
#include <vector>
#include <list>
#include <ctime>
#include <string>
#include <fstream>
#include <algorithm>
#include <climits>

#define LFU_TEST_GENERATING_
#define PERFECT_TEST_GENERATING_

namespace Tests {
    using u_int = std::size_t;
//------------------------------------------------------------------------------------------//
class weak_lfu {
    using Key = u_int;
    using cacheType = std::pair<Key, u_int>;
    using cacheIter = typename std::list<cacheType>::iterator;

    cacheIter find_minimum_freq(); //find key with minimum frequency
    bool is_full() const noexcept;
    cacheIter find_last(u_int freq);
    cacheIter find(Key key);
public:
    weak_lfu(u_int capacity);
    weak_lfu() = default;

    bool lookup_update(Key key);
private:
    std::list<cacheType> cache_;
    u_int cache_size_;
    u_int capacity_;
};

weak_lfu::weak_lfu(u_int capacity):
        cache_size_{0}, capacity_{capacity} {}

bool weak_lfu::lookup_update(Key key) {
    if (!capacity_) {
        return false;
    }
    auto cache_iter = find(key);
    if (cache_iter == cache_.end()) { // not found
        if (is_full()) {
            cache_.pop_front();
        } else {
            ++cache_size_;
        }
        auto after_one_freq = std::find_if(cache_.begin(), cache_.end(), 
                [](auto&& iter) { return iter.second != 1; });
        cache_.emplace(after_one_freq, key, 1);
        return false;
    }

    cache_iter->second += 1;
    u_int freq = cache_iter->second;
    cache_.erase(cache_iter);
    auto emplace_iter = find_last(freq);
    if (emplace_iter == cache_.end()) {
        cache_.emplace_back(key, freq);
    } else {
        cache_.emplace(emplace_iter, key, freq);
    }
    return true;
}

weak_lfu::cacheIter weak_lfu::find_last(u_int freq) {
    auto ret_value = std::find_if(cache_.rbegin(), cache_.rend(),
                        [&freq](auto&& it) { return it.second <= freq; });
    return ret_value.base();
}

weak_lfu::cacheIter weak_lfu::find(Key key) {
    return std::find_if(cache_.begin(), cache_.end(),
                        [&key](auto&& it) { return it.first == key; });
}

bool weak_lfu::is_full() const noexcept {
    return cache_size_ == capacity_;
}

weak_lfu::cacheIter weak_lfu::find_minimum_freq() {
    return std::min_element(cache_.begin(), cache_.end(), 
                    [](auto&& it1, auto&& it2) { return it1.second < it2.second; });
}
//------------------------------------------------------------------------------------------//

class weak_perfect {
    using Key       = u_int;
    using cacheIter = typename std::vector<Key>::iterator;
    
    cacheIter find(Key key);
    bool is_full() const noexcept;
    cacheIter find_farthest_value(cacheIter buff_it);
public:
template<typename Iter>
    weak_perfect(u_int capacity, Iter begin, Iter end);
    
    void lookup_update();
    u_int get_hits() const noexcept;
private:
    u_int capacity_;
    std::vector<Key> buffer_;
    std::vector<Key> cache_;
    u_int hits_;
};

template<typename Iter>
weak_perfect::weak_perfect(u_int capacity, Iter begin, Iter end): 
               capacity_{capacity}, buffer_{begin, end}, hits_{0} {
    cache_.reserve(capacity_);
}

void weak_perfect::lookup_update() {
    if (!capacity_) {
        return ;
    }
    for (auto buff_it = buffer_.begin(); buff_it != buffer_.end(); ++buff_it) {
        auto pos_it = find(*buff_it);
        if (pos_it == cache_.end()) {
            if (is_full()) {
                auto most_far = find_farthest_value(buff_it);
                *most_far = *buff_it; 
            } else {
                cache_.push_back(*buff_it);
            }
        } else {
            ++hits_;
        }
    }
}

bool weak_perfect::is_full() const noexcept {
    return cache_.size() == capacity_;
}

weak_perfect::cacheIter weak_perfect::find_farthest_value(cacheIter buff_it) {
    std::vector<cacheIter> distance_iters{};
    for (cacheIter it1 = cache_.begin(); it1 != cache_.end(); ++it1) {
        auto key_iter = std::find(buff_it, buffer_.end(), *it1);
        if (key_iter == buffer_.end()) {
            return it1;
        }
        distance_iters.push_back(key_iter);
    }
    if (distance_iters.empty()) {
        return cache_.begin();
    }
    std::sort(distance_iters.begin(), distance_iters.end(), std::greater<cacheIter>());
    Key key = *distance_iters.front();
    return std::find(cache_.begin(), cache_.end(), key);
}

weak_perfect::cacheIter weak_perfect::find(Key key) {
    return std::find(cache_.begin(), cache_.end(), key); 
}

std::size_t weak_perfect::get_hits() const noexcept {
    return hits_;
}
//------------------------------------------------------------------------------------------//

class generator {
    using u_int = std::size_t;
    
    const u_int MAX_CACHE_SIZE     = 100;
    const u_int MAX_DATA_SIZE      = 1000000;
    const u_int MAX_PERF_DATA_SIZE = 100000;
    const u_int MIN_DATA_SIZE      = 150;
    const u_int MAX_DATA_VALUE     = 1000;
    const u_int MIN_DATA_VALUE     = 1;

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
    for (u_int count = 1; count <= test_number; ++count) {
#ifdef LFU_TEST_GENERATING_
        generate_lfu_files(count);
#endif
#ifdef PERFECT_TEST_GENERATING_
        generate_perfect_files(count);
#endif
    }
}

void generator::generate_lfu_files(u_int test_number) {
    std::string test_number_str = std::to_string(test_number);
    std::string test_file_name = "test" + test_number_str + ".txt";
    std::ofstream test_file("../lfu_resources/tests/" + test_file_name);

    u_int cache_cap = std::rand() % MAX_CACHE_SIZE;
    u_int data_size = (std::rand() * std::rand() + MIN_DATA_SIZE) % MAX_DATA_SIZE;

    weak_lfu cache(cache_cap);

    test_file << cache_cap << ' ' << data_size;
    u_int hits = 0;
    
    for (u_int count = 0; count < data_size; ++count) {
        u_int key = (std::rand() + MIN_DATA_VALUE) % MAX_DATA_VALUE;
        hits += cache.lookup_update(key);
        test_file << ' ' <<  key;
    }
    test_file << ' ';

    std::string answ_name = "../lfu_resources/answers/answ" + test_number_str + ".txt";
    std::ofstream answer(answ_name);
    answer << hits << std::endl;
}

void generator::generate_perfect_files(u_int test_number) {
    std::string test_number_str = std::to_string(test_number);
    std::string test_file_name  = "test" + test_number_str;
    std::ofstream test_file("../perfect_resources/tests/" + test_file_name + ".txt");

    u_int cache_cap = std::rand() % MAX_CACHE_SIZE;
    u_int data_size = (std::rand() + MIN_DATA_SIZE) % MAX_PERF_DATA_SIZE;
    
    std::vector<u_int> data{};
    data.reserve(data_size);

    test_file << cache_cap << ' ' << data_size;
    for (u_int count = 0; count < data_size; ++count) {
        u_int key = (std::rand() + MIN_DATA_VALUE) % MAX_DATA_VALUE;
        data.push_back(key);
        test_file << ' ' <<  key;
    }
    test_file << ' ';

    weak_perfect cache(cache_cap, data.begin(), data.end());
    cache.lookup_update();
    
    std::string answ_name = "../perfect_resources/answers/answ" + test_number_str + ".txt";
    std::ofstream answer(answ_name);
    answer << cache.get_hits() << std::endl;
}

}; // <-- namespace Tests
