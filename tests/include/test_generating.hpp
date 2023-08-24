#include <iostream>
#include <vector>
#include <list>
#include <random>
#include <ctime>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>


namespace Tests {
    using u_int = std::size_t;
//------------------------------------------------------------------------------------------//
class weak_lfu final {
    static constexpr u_int MIN_CAPACITY = 1;

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
    const u_int capacity_;
};

weak_lfu::weak_lfu(u_int capacity):
        cache_size_{0}, 
        capacity_{std::max(MIN_CAPACITY, capacity)} {}

bool weak_lfu::lookup_update(Key key) {
    auto cache_iter = find(key);
    if (cache_iter == cache_.end()) { // not found
        if (is_full()) {
            cache_.pop_front();
        } else {
            ++cache_size_;
        }
        auto after_one_freq = std::find_if(cache_.begin(), cache_.end(),
                [](auto&& value) { return value.second != 1; }
                );
        cache_.emplace(after_one_freq, key, 1);
        return false;
    }

    ++cache_iter->second;
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
                        [freq](auto&& value) { return value.second <= freq; }
                        );
    return ret_value.base();
}

weak_lfu::cacheIter weak_lfu::find(Key key) {
    return std::find_if(cache_.begin(), cache_.end(),
                        [key](auto&& value) { return value.first == key; }
                        );
}

bool weak_lfu::is_full() const noexcept {
    return cache_size_ == capacity_;
}

weak_lfu::cacheIter weak_lfu::find_minimum_freq() {
    return std::min_element(cache_.begin(), cache_.end(),
                    [](auto&& elem1, auto&& elem2) { return elem1.second < elem2.second; }
                    );
}
//------------------------------------------------------------------------------------------//

class weak_perfect final {
    static constexpr u_int MIN_CAPACITY = 1;
    
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
    const u_int capacity_;
    std::vector<Key> buffer_;
    std::vector<Key> cache_;
    u_int hits_;
};

template<typename Iter>
weak_perfect::weak_perfect(u_int capacity, Iter begin, Iter end): 
               capacity_{std::max(MIN_CAPACITY, capacity)}, 
               buffer_{begin, end}, 
               hits_{0} {
    cache_.reserve(capacity_);
}

void weak_perfect::lookup_update() {
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

u_int weak_perfect::get_hits() const noexcept {
    return hits_;
}
//------------------------------------------------------------------------------------------//

namespace dirrs {
    const std::string lfu_dirr           = "../lfu_resources";
    const std::string lfu_dirr_tests     = "../lfu_resources/tests/";
    const std::string lfu_dirr_answs     = "../lfu_resources/answers/";
    const std::string perfect_dirr       = "../perfect_resources";
    const std::string perfect_dirr_tests = "../perfect_resources/tests/"; 
    const std::string perfect_dirr_answs = "../perfect_resources/answers/";
};

class generator final {
    static constexpr u_int MAX_TESTS_NUMBER = 150;
    
    static constexpr u_int MAX_CACHE_SIZE     = 100;
    static constexpr u_int MIN_CACHE_SIZE     = 1;
    static constexpr u_int MAX_DATA_SIZE      = 1000000;
    static constexpr u_int MAX_PERF_DATA_SIZE = 10000;
    static constexpr u_int MIN_DATA_SIZE      = 10;
    static constexpr u_int MAX_DATA_VALUE     = 1000;
    static constexpr u_int MIN_DATA_VALUE     = 1;
 
    using gener_type = std::mt19937;

    void init_generator(gener_type& generator);
    u_int random(gener_type& generator, u_int min_value, u_int max_value);
    
    void create_lfu_dirrs();
    void create_perfect_dirrs();

    void generate_lfu_files(u_int test_number, gener_type& generator);
    void generate_perfect_files(u_int test_number, gener_type& generator);
public: 
    void generate(u_int tests_number);
};

void generator::generate(u_int test_number) {
    gener_type generator;
    init_generator(generator);   
#ifdef PERFECT_TEST_GENERATING_    
    create_perfect_dirrs();
#else
    create_lfu_dirrs();
#endif
    if (test_number > MAX_TESTS_NUMBER) {
        test_number = MAX_TESTS_NUMBER;
    }
    for (u_int count = 1; count <= test_number; ++count) {
#ifdef PERFECT_TEST_GENERATING_
        generate_perfect_files(count, generator);
#else
        generate_lfu_files(count, generator);
#endif
    }
}

void generator::create_lfu_dirrs() {
    using namespace std::filesystem;
    directory_entry lfu_source_dirr(dirrs::lfu_dirr);
    if (!lfu_source_dirr.is_directory()) {
        create_directory(dirrs::lfu_dirr);
        create_directory(dirrs::lfu_dirr_tests);
        create_directory(dirrs::lfu_dirr_answs);
    } else {
        //revome old data
        const path tests_path{dirrs::lfu_dirr_tests};         
        const path answs_path{dirrs::lfu_dirr_answs};

        for (auto& dir_iter : directory_iterator{tests_path}) {
            remove(dir_iter.path());
        }
        for (auto& dir_iter : directory_iterator{answs_path}) {
            remove(dir_iter.path());
        }
    }
}

void generator::create_perfect_dirrs() {
    using namespace std::filesystem;
    directory_entry perfect_source_dirr(dirrs::perfect_dirr);
    if (!perfect_source_dirr.is_directory()) {
        create_directory(dirrs::perfect_dirr);
        create_directory(dirrs::perfect_dirr_tests);
        create_directory(dirrs::perfect_dirr_answs);
    } else {
        //revome old data 
        const path tests_path{dirrs::perfect_dirr_tests};         
        const path answs_path{dirrs::perfect_dirr_answs};

        for (auto& dir_iter : directory_iterator{tests_path}) {
            remove(dir_iter.path());
        }
        for (auto& dir_iter : directory_iterator{answs_path}) {
            remove(dir_iter.path());
        }
    }
}

void generator::generate_lfu_files(u_int test_number, gener_type& generator) {
    std::string test_number_str = std::to_string(test_number);
    std::string test_file_name = "test" + test_number_str + ".txt";

    std::ofstream test_file(dirrs::lfu_dirr_tests + test_file_name);

    u_int cache_cap = random(generator, MIN_CACHE_SIZE, MAX_CACHE_SIZE);
    u_int data_size = random(generator, MIN_DATA_SIZE, MAX_DATA_SIZE); 
    weak_lfu cache(cache_cap);

    test_file << cache_cap << ' ' << data_size;
    u_int hits = 0;
    
    for (u_int count = 0; count < data_size; ++count) {
        u_int key = random(generator, MIN_DATA_VALUE, MAX_DATA_VALUE);
        hits += cache.lookup_update(key);
        test_file << ' ' <<  key;
    }
    test_file << ' ';

    std::string answ_name = dirrs::lfu_dirr_answs + "answ" + test_number_str + ".txt";
    std::ofstream answer(answ_name);
    answer << hits << std::endl;
}

void generator::generate_perfect_files(u_int test_number, gener_type& generator) {
    std::string test_number_str = std::to_string(test_number);
    std::string test_file_name  = "test" + test_number_str;
    
    std::ofstream test_file(dirrs::perfect_dirr_tests + test_file_name + ".txt");

    u_int cache_cap = random(generator, MIN_CACHE_SIZE, MAX_CACHE_SIZE);
    u_int data_size = random(generator, MIN_DATA_SIZE, MAX_PERF_DATA_SIZE);
    
    std::vector<u_int> data{};
    data.reserve(data_size);

    test_file << cache_cap << ' ' << data_size;
    for (u_int count = 0; count < data_size; ++count) {
        u_int key = random(generator, MIN_DATA_VALUE, MAX_DATA_VALUE);
        data.push_back(key);
        test_file << ' ' <<  key;
    }
    test_file << ' ';

    weak_perfect cache(cache_cap, data.begin(), data.end());
    cache.lookup_update();
    
    std::string answ_name = dirrs::perfect_dirr_answs + "answ" + test_number_str + ".txt";
    std::ofstream answer(answ_name);
    answer << cache.get_hits() << std::endl;
}


void generator::init_generator(gener_type& generator) {
    const u_int seed = static_cast<u_int>(std::time(nullptr));
    generator.seed(seed);
}

u_int generator::random(gener_type& generator, u_int min_value, u_int max_value) {
    std::uniform_int_distribution<u_int> distribution(min_value, max_value);
    return distribution(generator);
}

}; // <-- namespace Tests
