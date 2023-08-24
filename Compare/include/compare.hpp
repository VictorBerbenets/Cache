#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <stdexcept>

#include "lfu_cache.hpp"
#include "perfect_cache.hpp"

namespace yLAB {

template<typename T, typename KeyT = int>
class compare final {
public:
    compare(std::size_t caches_capacity, std::istream& is);
    ~compare() = default;

    void dump(std::string& test_file_name);

    std::string& get_clean_file_name(std::string& full_file_name);

    double get_lfu_time() const;
    double get_perfect_time() const;
    std::size_t get_lfu_hits() const noexcept;
    std::size_t get_perfect_hits() const noexcept;
private:
    lfu_cache<T, KeyT> lfu_;
    perfect_cache<T, KeyT> perfect_;
    std::size_t lfu_hits_ = 0, perfect_hits_ = 0;
    std::chrono::duration<double> lfu_time_, perfect_time_;

    std::size_t data_size_;
    std::size_t capacity_;
}; // <-- class compare

template<typename T, typename KeyT>
compare<T, KeyT>::compare(std::size_t caches_cap, std::istream& is):
                lfu_{caches_cap},
                perfect_{caches_cap},
                capacity_{caches_cap} {
    using size_type = std::size_t;

    is >> data_size_;
    std::vector<size_type> data{};
    data.reserve(data_size_);
    //saving data
    for (size_type count = 0; count < data_size_; ++count) {
        size_type tmp1{};
        is >> tmp1;
        data.push_back(tmp1);
    }
    //checking lfu cache
    using page_t = typename lfu_cache<T, KeyT>::page_t;
    auto get_page = [](KeyT key) -> page_t {
                return page_t {T{}, key};
                };

    auto lfu_start = std::chrono::high_resolution_clock::now();
    for (auto iter : data) {
        lfu_hits_ += lfu_.lookup_update(iter, get_page);//lfu hits
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
std::string& compare<T, KeyT>::get_clean_file_name(std::string& file_name) {
    for (auto r_it = file_name.rbegin(); r_it != file_name.rend(); ++r_it) {
        if (*r_it == '/' || *r_it == '\\') {
            file_name.erase(file_name.begin(), r_it.base());
            break;
        }
    }
    return file_name;
}

template<typename T, typename KeyT>
void compare<T, KeyT>::dump(std::string& test_file_name) {
    std::string comp_file = "comparing.txt";
    std::ofstream dump_file(comp_file, std::ios::app);
    if (!dump_file.is_open()) {
        throw std::invalid_argument{"error while opening compare file\n"};
    }
    std::string clean_file_name = get_clean_file_name(test_file_name);

    dump_file << "----------------------------------------------------------------\n";
    dump_file << "                         file name:      " << clean_file_name << '\n';
    dump_file << "                         cache capacity: " << capacity_ << '\n';
    dump_file << "                         data_size:      " << data_size_ << '\n';
    dump_file << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n";
    dump_file << "                              Hits                              \n";
    dump_file << "lfu: " << get_lfu_hits() << "\nperfect: " << get_perfect_hits() << '\n';
    dump_file << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - \n";
    dump_file << "                              Time                              \n";
    dump_file << "lfu: " << get_lfu_time() << "\nperfect: " << get_perfect_time() << '\n';
    dump_file << "----------------------------------------------------------------\n\n";
    if (!dump_file.good()) {
        throw std::runtime_error{"writing file error\n!"};
    }
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
