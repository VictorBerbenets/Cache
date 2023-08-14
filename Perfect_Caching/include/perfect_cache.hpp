#ifndef _PERFECT_CACHE_
#define _PERFECT_CACHE_

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <list>
#include <vector>


namespace Perfect_Cache {
    using size_type = std::size_t;

template<typename Key, typename InfoType = std::string>
struct page_t {
    Key key_;
    InfoType data_;
};

template<typename T, typename KeyT = int>
class cache {
    using cacheCell   = std::pair<KeyT, T>;
    using cacheIter   = typename std::list<cacheCell>::iterator;
    using value_count = std::deque< std::pair<size_type, cacheIter> >;

    void fill_buffers(std::istream& is, size_type pages_number);
    void fill_cache();
public:
    cache(size_type capacity, std::istream& is);
    ~cache() = default;

    bool is_full() const noexcept;
    size_type get_hits() const noexcept;
    void clear() noexcept;
private:
    size_type cache_size_;
    const size_type capacity_;
    std::vector<cacheCell> cache_;
    std::unordered_set<KeyT> cache_checker_; //for checking if element already in cache
    std::list<cacheCell> ordered_buffer_; //for saving data in input order
    std::unordered_map<KeyT, value_count> unordered_buffer_; // save all data for finding  

    size_type hits_;
};

template<typename T, typename KeyT>
cache<T, KeyT>::cache(size_type capacity, std::istream& is):
                cache_size_{0}, capacity_{capacity} {
    cache_.reserve(capacity_);

    size_type pages_number{};
    is >> pages_number;

    fill_buffers(is, pages_number);
    fill_cache();

}

template<typename T, typename KeyT>
void cache<T, KeyT>::fill_buffers(std::istream& is, size_type pages_number) {
    page_t<KeyT, T> tmp{};
    for (size_type count = 1; count <= pages_number; ++count) {
        is >> tmp.key_;
        ordered_buffer_.push_back({tmp.key_, tmp.data_}); //ordered pages 
        //saving page order by page number to std::deque
        unordered_buffer_[tmp.key_].push_back( std::pair{count, std::prev(ordered_buffer_.end())} );
    }
}

template<typename T, typename KeyT>
void cache<T, KeyT>::fill_cache() {
    for (auto& buff_iter : ordered_buffer_) {
        //if element not in cache
        if (cache_checker_.find(buff_iter.first) == cache_checker_.end()) {
            cache_checker_.insert(buff_iter.first);
//            cache_.push_back( 
        }
    }
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
    unordered_buffer_.clear();
    cache_size_ = 0;
    capacity_   = 0;
    hits_       = 0;
}

};


#endif
