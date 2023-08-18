#ifndef PERFECT_CACHE_
#define PERFECT_CACHE_

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>
#include <vector>
#include <stdexcept>

namespace Perfect_Cache {
    using size_type = std::size_t;

template<typename InfoType = std::string, typename Key = size_type>
struct page_t {
    Key key_;
    InfoType data_;
};

template<typename T, typename KeyT = int>
class cache {
    using cacheCell      = std::pair<KeyT, T>;
    using cacheIter      = typename std::vector<cacheCell>::iterator;
    using elements_order = std::deque<size_type>;
 
    void fill_buffers(std::istream& is, size_type pages_number);
template<typename Iter>
    void fill_buffers(Iter first, Iter last);
    void fill_cache();
    void replace_cache_value(cacheIter cache_iter, cacheCell&& replacement);
    void remove_value_entry_number(KeyT key);
    void push_cache(cacheCell&& push_value);
    cacheIter find_furthest_value();
public:
    cache(size_type capacity);
    ~cache() = default;
    
    void give_data(std::istream& is);
template<typename Iter>
    void give_data(Iter first, Iter last);

    bool is_full() const noexcept;
    size_type get_hits() const noexcept;
    void print_cache() const;
    void clear() noexcept;
private:
    size_type cache_size_;
    const size_type capacity_;
    std::vector<cacheCell> cache_;
    std::unordered_set<KeyT> cache_checker_; //for checking if element already in cache
    std::list<cacheCell> ordered_buffer_; //for saving data in input order
    std::unordered_map<KeyT, elements_order> unordered_buffer_; // save all data for finding  

    size_type hits_;
};

template<typename T, typename KeyT>
cache<T, KeyT>::cache(size_type capacity):
                cache_size_{0}, capacity_{capacity} {
    cache_.reserve(capacity_);
}

template<typename T, typename KeyT>
void cache<T, KeyT>::give_data(std::istream& is) {
    size_type pages_number = 0;
    is >> pages_number;
    if (!is.good()) {
        throw std::runtime_error{"Data size reading error!\n"};
    }

    fill_buffers(is, pages_number);
    fill_cache();
}

template<typename T, typename KeyT>
template<typename Iter>
void cache<T, KeyT>::give_data(Iter first, Iter last) {
    fill_buffers(first, last);
    fill_cache();
}

template<typename T, typename KeyT>
void cache<T, KeyT>::fill_buffers(std::istream& is, size_type pages_number) {
    page_t<T, KeyT> tmp{};
    for (size_type count = 1; count <= pages_number; ++count) {
        is >> tmp.key_;
        if (!is.good()) {
            throw std::runtime_error{"Data value reading error!\n"};
        }
        ordered_buffer_.emplace_back(tmp.key_, tmp.data_); //ordered pages 
        //saving page order by page number to std::deque
        unordered_buffer_[tmp.key_].push_back(count);
    }
}


template<typename T, typename KeyT>
template<typename Iter>
void cache<T, KeyT>::fill_buffers(Iter first, Iter last) {
    page_t<T, KeyT> tmp{};
    for (size_type count = 1; first != last; ++first, ++count) {
        tmp.key_ = *first;
        ordered_buffer_.emplace_back(tmp.key_, tmp.data_); //ordered pages 
        //saving page order by page number to std::deque
        unordered_buffer_[tmp.key_].push_back(count);
    }
}

template<typename T, typename KeyT>
void cache<T, KeyT>::fill_cache() {
    for (auto& buff_iter : ordered_buffer_) {
        //if element not in cache
        if (cache_checker_.find(buff_iter.first) == cache_checker_.end()) {
           // remember new cache element 
            cache_checker_.insert(buff_iter.first);
            if (is_full()) {
                auto replace_iter = find_furthest_value();
                replace_cache_value(replace_iter, {buff_iter.first, buff_iter.second});
            } else {
                push_cache({buff_iter.first, buff_iter.second});
            }
        } else {
            ++hits_;
        }
        //after pushing elem to cache we don't need it in hash_table 
        remove_value_entry_number(buff_iter.first);
    }
}

template<typename T, typename KeyT>
typename cache<T, KeyT>::cacheIter cache<T, KeyT>::find_furthest_value() {
    size_type max_distance = 0;
    typename std::vector<cacheCell>::iterator replace_iter;
    for (auto cache_iter = cache_.begin(); cache_iter != cache_.end(); ++cache_iter) {
        if (unordered_buffer_.find(cache_iter->first) != unordered_buffer_.end()) {
            auto deque_iter = unordered_buffer_[cache_iter->first].begin();
            if (*deque_iter > max_distance) {
                max_distance = *deque_iter;
                replace_iter = cache_iter;
            }
        } else {
            replace_iter = cache_iter;
            break;
        }
    }
    return replace_iter;
}

template<typename T, typename KeyT>
void cache<T, KeyT>::replace_cache_value(cacheIter cache_iter, cacheCell&& replacement) {
    cache_checker_.erase(cache_iter->first);
    *cache_iter = replacement;
}

template<typename T, typename KeyT>
void cache<T, KeyT>::remove_value_entry_number(KeyT key) {
    unordered_buffer_[key].pop_front();
    if (unordered_buffer_[key].empty()) {
        unordered_buffer_.erase(key);
    }
}

template<typename T, typename KeyT>
void cache<T, KeyT>::push_cache(cacheCell&& push_value) {
    cache_.push_back(push_value);
    ++cache_size_;
}

template<typename T, typename KeyT>
bool cache<T, KeyT>::is_full() const noexcept {
    return cache_size_ == capacity_;
}

template<typename T, typename KeyT>
size_type cache<T, KeyT>::get_hits() const noexcept {
    return hits_;
}

template<typename T, typename KeyT>
void cache<T, KeyT>::clear() noexcept {
    cache_.clear();
    ordered_buffer_.clear();
    cache_checker_.clear();
    unordered_buffer_.clear();
    cache_size_ = 0;
    capacity_   = 0;
    hits_       = 0;
}

template<typename T, typename KeyT>
void cache<T, KeyT>::print_cache() const {
    std::cout << "cache: ";
    for (auto& it : cache_) {
        std::cout << it.first << ' ';
    }
    std::cout << '\n';
}

}; 

#endif

