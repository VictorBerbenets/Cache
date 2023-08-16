#include <iostream>
#include <vector>
#include <chrono>
#include "cache.hpp"
#include "perfect_cache.hpp"

namespace Caches_Comporator {

template<typename T, typename KeyT = int>
class compare {
    using lfu_cache = Cache_LFU::cache<T, KeyT>;
    using perfect_cache = Perfect_Cache::cache<T, KeyT>;

public:
    compare(std::size_t caches_capacity, std::istream& is);
    ~compare() = default;
    
    void dump(std::ofstream& o_file) const noexcept;

    double get_lfu_time() const noexcept;
    double get_perfect_time() const noexcept;
    std::size_t get_lfu_hits() const noexcept;
    std::size_t get_perfect_hits() const noexcept;
private:
    lfu_cache lfu_;
    perfect_cache perfect_;
    
    std::size_t lfu_hits_, perfect_hits_;
    std::chrono::duration<double> lfu_time_, perfect_time_;
};

template<typename T, typename KeyT>
compare<T, KeyT>::compare(std::size_t caches_capacity, std::istream& is): 
                lfu_(caches_capacity), perfect_(caches_capacity) {
    using type = std::size_t;

    type data_size{};
    is >> data_size;
    std::vector<type> data{};
    data.reserve(data_size);
    //saving data
    type tmp{};
    for (type count = 0; count < data_size; ++count) {
        is >> tmp;
        data.push_back(tmp);
    }
    //checking lfu cache
    auto lfu_start = std::chrono::high_resolution_clock::now();
    for (auto iter : data) {
        Cache_LFU::page_t tmp{iter, ""};
        lfu_hits_ += lfu_.lookup_update(iter, tmp);//lfu hits
    }
    auto lfu_end = std::chrono::high_resolution_clock::now();//lfu time
    lfu_time_ = lfu_end - lfu_start;
    //checking perfect cache 
    auto perfect_start = std::chrono::high_resolution_clock::now();
    perfect_.give_data(data.begin(), data.end()); 
    perfect_hits_ = perfect_.get_hits();
    auto perfect_end = std::chrono::high_resolution_clock::now();
    perfect_time_ = perfect_end - perfect_start; 
}

template<typename T, typename KeyT>
void compare<T, KeyT>::dump(std::ofstream& o_file) const noexcept{
    
}

template<typename T, typename KeyT>
std::size_t compare<T, KeyT>::get_lfu_hits() const noexcept {
    return lfu_hits_;
}

template<typename T, typename KeyT>
std::size_t compare<T, KeyT>::get_perfect_hits() const noexcept {
    return perfect_hits_;
}

template<typename T, typename KeyT>
double compare<T, KeyT>::get_perfect_time() const noexcept {
    return perfect_time_.count();
}

template<typename T, typename KeyT>
double compare<T, KeyT>::get_lfu_time() const noexcept {
    return lfu_time_.count();
}

};
