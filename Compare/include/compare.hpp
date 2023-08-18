#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

#include "cache.hpp"
#include "perfect_cache.hpp"

namespace yLAB {

template<typename T, typename KeyT = int>
class compare {
public:
    compare(std::size_t caches_capacity, std::istream& is);
    ~compare() = default;
    
    void dump(const std::string& test_file_name) const;

    double get_lfu_time() const; 
    double get_perfect_time() const;
    std::size_t get_lfu_hits() const noexcept;
    std::size_t get_perfect_hits() const noexcept;
private:
    lfu_cache<T, KeyT> lfu_;
    perfect_cache<T, KeyT> perfect_;
    std::size_t lfu_hits_, perfect_hits_;
    std::chrono::duration<double> lfu_time_, perfect_time_;
    
    std::size_t data_size_;
    std::size_t capacity_;
}; // <-- class compare

template<typename T, typename KeyT>
compare<T, KeyT>::compare(std::size_t caches_cap, std::istream& is): 
                lfu_{caches_cap}, perfect_{caches_cap}, capacity_{caches_cap} {
    using size_type = std::size_t;

    is >> data_size_;
    std::vector<size_type> data{};
    data.reserve(data_size_);
    //saving data
    size_type tmp1{};
    for (size_type count = 0; count < data_size_; ++count) {
        is >> tmp1;
        data.push_back(tmp1);
    }
    //checking lfu cache
    auto lfu_start = std::chrono::high_resolution_clock::now();
    for (auto iter : data) {
        T value{};
        std::pair<T, KeyT> tmp2(value, iter);
        lfu_hits_ += lfu_.lookup_update(iter, tmp2);//lfu hits
    }
    auto lfu_end = std::chrono::high_resolution_clock::now();//lfu time
    lfu_time_    = lfu_end - lfu_start;
    //checking perfect cache 
    auto perfect_start = std::chrono::high_resolution_clock::now();
    perfect_.give_data(data.begin(), data.end()); 
    perfect_hits_    = perfect_.get_hits();
    auto perfect_end = std::chrono::high_resolution_clock::now();
    perfect_time_    = perfect_end - perfect_start; 
}

template<typename T, typename KeyT>
void compare<T, KeyT>::dump(const std::string& test_file_name) const {
    std::string comp_file = "../comparing.txt";
    std::ofstream dump_file(comp_file, std::ios::app);

    dump_file << "----------------------------------------------------------------\n";
    dump_file << "                         file name:      " << test_file_name << '\n';
    dump_file << "                         cache capacity: " << capacity_ << '\n';
    dump_file << "                         data_size:      " << data_size_ << '\n';
    dump_file << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n";
    dump_file << "                              Hits                              \n";
    dump_file << "lfu: " << get_lfu_hits() << "\nperfect: " << get_perfect_hits() << '\n';
    dump_file << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n";
    dump_file << "                              Time                              \n";
    dump_file << "lfu: " << get_lfu_time() << "\nperfect: " << get_perfect_time() << '\n';
    dump_file << "----------------------------------------------------------------\n\n";
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
double compare<T, KeyT>::get_perfect_time() const {
    return perfect_time_.count();
}

template<typename T, typename KeyT>
double compare<T, KeyT>::get_lfu_time() const {
    return lfu_time_.count();
}

}; // <-- namespace yLAB
