#ifndef _PERFECT_CACHE_
#define _PERFECT_CACHE_

#include <iostream>
#include <unordered_map>
#include <deque>
#include <vector>


namespace Perfect_Cache {
    using size_type = std::size_t;

template<typename Key, typename InfoType = std::string>
struct page_t {
    Key key_;
    InfoType info_;
};

template<typename T, typename KeyT = int>
class cache {
    using cacheCell   = std::pair<KeyT, T>;
    using value_count = std::pair< std::deque<size_type>, T >;
    using buffer_type = std::unordered_map<KeyT, value_count>; 

    void fill_buffer(std::istream& is, size_type pages_number);
    void fill_cache();
public:
    cache(size_type capacity, std::istream& is);

    bool is_full() const noexcept;
    size_type get_hits() const noexcept;
    
private:
    size_type cache_size_;
    const size_type capacity_;
    std::vector<cacheCell> cache_;
    buffer_type buffer_;
    
    size_type hits_;
};

template<typename T, typename KeyT>
cache<T, KeyT>::cache(size_type capacity, std::istream& is):
                cache_size_{0}, capacity_(capacity) {
    size_type pages_number{};
    is >> pages_number;

    fill_buffer(is, pages_number);

    fill_cache();

}

template<typename T, typename KeyT>
void cache<T, KeyT>::fill_buffer(std::istream& is, size_type pages_number) {
    page_t<KeyT, T> tmp{};
    for (size_type count = 1; count <= pages_number; ++count) {
        is >> tmp.key_;
        //saving page order by page number to std::deque
        buffer_[tmp.key_].first.push_back(count);
    }
}

template<typename T, typename KeyT>
void cache<T, KeyT>::fill_cache() {

}

template<typename T, typename KeyT>
bool cache<T, KeyT>::is_full() const noexcept {
    return cache_size_ == capacity_;
}
template<typename T, typename KeyT>
size_type cache<T, KeyT>::get_hits() const noexcept {
    return hits_;
}


};


#endif
