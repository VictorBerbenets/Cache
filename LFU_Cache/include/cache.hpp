#ifndef LFU_CACHE__
#define LFU_CACHE__

#include <iostream>
#include <unordered_map>
#include <list>

//|----------------------------|//
//|    LFU cache algorithm     |//
//|----------------------------|//

namespace Cache_LFU { 
using size_type = std::size_t;

template<typename T, typename Key>
struct page_t {
    Key page_id;
    T page;
};

template<typename T, typename KeyT = int>
class cache {
    struct item;
    struct frequencyItem;
    
    using freqIter   = typename std::list<frequencyItem>::iterator;
    using itemIter   = typename std::list<item>::iterator;  
    using cacheValue = page_t<T, KeyT>;
    
    void insert_item(KeyT key, const cacheValue& value, freqIter iter);
    void move_item_element(KeyT key, const cacheValue& value, freqIter cache_iter, 
                           freqIter pos_to_insert, itemIter pos_to_erase);
    void remove_last_item();
public:
    explicit cache(size_type capacity);
    ~cache() = default;

    bool is_full() const noexcept;
    bool lookup_update(KeyT key, const cacheValue& value);
    void print_cache() const noexcept;
private:
    size_type cache_size_; 
    const size_type capacity_;
    std::unordered_map<KeyT, itemIter> hash_table_;
    std::list<frequencyItem> cache_;

    struct item {
        KeyT key_;
        cacheValue value_;
        freqIter fr_iter_;
 
        item(KeyT key, const cacheValue& value, freqIter it):
            key_{key}, value_{value}, fr_iter_{it} {};
    };
    
    struct frequencyItem {
        const size_type freq_;   
        std::list<item> freq_list_;

        explicit frequencyItem(size_type freq):
            freq_{freq} {};
    };
};

template<typename T, typename KeyT>
cache<T, KeyT>::cache(size_type capacity): 
    cache_size_(0), capacity_(capacity) {};

template<typename T, typename KeyT>
bool cache<T, KeyT>::lookup_update(KeyT key, const cacheValue& value) {
    if (!capacity_) {
        std::cout << "Cache is not useful: cache's capacity == 0" << '\n';
        return false;
    }
    auto is_found = hash_table_.find(key);
    if (is_found == hash_table_.end()) {
        if (is_full()) {    
            remove_last_item();
            //freq-node with member freq_ = 1 must exist because we got new element
            if ( cache_.begin()->freq_ != 1 ) {
                cache_.emplace_front(1);
            }
            //insert item in front of the freq-list with begin iterator
            insert_item(key, value, cache_.begin());
            return false;
        }
        //if cache has't freq-list with freq_ == 1
        if (cache_.begin()->freq_ != 1 || !cache_size_) {
            cache_.emplace_front(1);                
        }
        insert_item(key, value, cache_.begin());
        ++cache_size_;
        return false;
    } 
   //element was found in hash_table //
   //get Iter of list with found element
    freqIter cache_iter  = (is_found->second)->fr_iter_;
    auto next_cache_iter = std::next(cache_iter);
    //looking for next suitable frequencyItem: if next one isn't suitable than insert your own
    if (next_cache_iter != cache_.end() && next_cache_iter->freq_ == (cache_iter->freq_ + 1)) {
        move_item_element(key, value, cache_iter, next_cache_iter, is_found->second);
    } else {
        freqIter inserted_cell = cache_.insert(next_cache_iter, frequencyItem{cache_iter->freq_ + 1}); 
        move_item_element(key, value, cache_iter, inserted_cell, is_found->second);
    }
    return true;
}

template<typename T, typename KeyT>
bool cache<T, KeyT>::is_full() const noexcept {
    return cache_size_ == capacity_;
}

template<typename T, typename KeyT>
void cache<T, KeyT>::insert_item(KeyT key, const cacheValue& value, freqIter new_freq_iter) {
    (new_freq_iter->freq_list_).emplace_front(key, value, new_freq_iter);  //push new item in the frequencyItem-list                        
    hash_table_[key] = (new_freq_iter->freq_list_).begin(); //saving new item iter                                                            
}

template<typename T, typename KeyT>
void cache<T, KeyT>::move_item_element(KeyT key, const cacheValue& value, freqIter cache_iter, 
     freqIter pos_to_insert, itemIter pos_to_erase) {
        (cache_iter->freq_list_).erase(pos_to_erase);
        insert_item(key, value, pos_to_insert);
        if ((cache_iter->freq_list_).empty()) {
            cache_.erase(cache_iter);
        }
}

template<typename T, typename KeyT>
void cache<T, KeyT>::remove_last_item() { 
    //remove last element in the first freq-list node
    auto key_to_remove = std::prev((cache_.begin()->freq_list_).end())->key_;
    hash_table_.erase(key_to_remove);
    (cache_.begin()->freq_list_).pop_back();
    if ( (cache_.begin()->freq_list_).empty() ) {
        cache_.pop_front();
    }
}

template<typename T, typename KeyT>
void cache<T, KeyT>::print_cache() const noexcept{
    std::cout << "cache:\n";
    for (const auto& it1 : cache_) {
        std::cout << it1.freq_ << ":";
        for (const auto& it2 : it1.freq_list_) {
            std::cout << it2.key_ << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "\n\n";
}

}
#endif

