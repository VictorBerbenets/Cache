#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <stack>

//|----------------------------|//
//|    LFU cache algorithm     |//
//|----------------------------|//

namespace Cache_LFU { 
using size_type = std::size_t;

struct page_t {
    size_type page_id;
    std::string page;
};

template<typename T, typename KeyT = int>
class cache {
    struct item;
    struct frequencyItem;
    
    using freqIter = typename std::list<frequencyItem>::iterator;
public:
    explicit cache(size_type capacity);
    ~cache() = default;

    bool isFull() const noexcept;
    bool inCache(KeyT key) const;

    bool lookup_update(KeyT key, const T& value);
    freqIter get_cache_iter(size_type offset);

private:

    size_type cache_size_; 
    size_type capacity_;
    std::unordered_map<KeyT, item*> hash_table_;
    std::list<frequencyItem> cache_;

    struct item {
        KeyT key_;
        T value_;
//        frequencyItem* freq_ptr_;    
        freqIter fr_iter_;

        item(KeyT key, const T& value):
            key_{key}, value_{value} {};
        
        item(KeyT key, const T& value, freqIter it):
            key_{key}, value_{value}, fr_iter_{it} {};
    };
    
    struct frequencyItem {
        size_type freq_;   
        std::stack<item> freq_list_;

        explicit frequencyItem(size_type freq):
            freq_{freq} {};
        void inc_frequency() {
            ++freq_;
        }
    };
    /*class frequencyItem {
        std::size_t freq_;   
        std::map<size_type, std::list<item>> freq_list_;
    };*/
};

template<typename T, typename KeyT>
cache<T, KeyT>::cache(size_type capacity): 
    cache_size_(0), capacity_(capacity) {};

template<typename T, typename KeyT>
bool cache<T, KeyT>::isFull() const noexcept {
    return cache_size_ == capacity_;
}

template<typename T, typename KeyT>
typename cache<T, KeyT>::freqIter 
cache<T, KeyT>::get_cache_iter(size_type offset) {
    if (offset > cache_size_) {
        std::cout << "invalid iterator offset\n";
        return cache_.end();
    }
    auto ret_it = cache_.begin();
    std::advance(ret_it, offset);
    return ret_it;
}

template<typename T, typename KeyT>
bool cache<T, KeyT>::lookup_update(KeyT key, const T& value) {
    auto is_found = hash_table_.find(key);
    if (is_found == hash_table_.end()) {
//            auto get_cache_first = get_cache_iter(0);    
        if (cache_.isFull()) { 
            auto get_cache_first = cache_.begin();
            (*get_cache_first).freq_list_.pop();
        } else if (!cache_size_) {
            cache_.push_front(frequencyItem{1});            
        }
        item it{key, value, get_cache_first};
        (*get_cache_first).freq_list_.push(it);
        ++cache_size_;
    } 
}

template<typename T, typename KeyT>
bool cache<T, KeyT>::inCache(KeyT key) const {
    
}

size_type CheckHits(const cache<page_t>& cch, size_type number) {
    size_type hits = 0;
    for (size_type count = 0; count < number; ++count) {
        page_t tmp_page {};
        std::cin >> tmp_page.page_id;
        //
    }
    return hits;
}

}



