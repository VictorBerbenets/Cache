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

struct page_t {
    size_type page_id;
    std::string page;
};

template<typename T, typename KeyT = int>
class cache {
    struct item;
    struct frequencyItem;
    
    using freqIter = typename std::list<frequencyItem>::iterator;
    using itemIter = typename std::list<item>::iterator;    
    
    void insert_item(KeyT key, const T& value, freqIter iter);
    void move_item_element(KeyT key, const T& value, freqIter cache_iter, 
                           freqIter pos_to_insert, itemIter pos_to_erase);
public:
    explicit cache(size_type capacity);
    ~cache() = default;

    bool is_full() const noexcept;
    bool lookup_update(KeyT key, const T& value);
    void print_cache() const noexcept;
    void print_hash_table() const noexcept;
private:
    size_type cache_size_; 
    const size_type capacity_;
    std::unordered_map<KeyT, itemIter> hash_table_;
    std::list<frequencyItem> cache_;

    struct item {
        KeyT key_;
        T value_;
        freqIter fr_iter_;
 
        item(KeyT key, const T& value, freqIter it):
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
bool cache<T, KeyT>::is_full() const noexcept {
    return cache_size_ == capacity_;
}

template<typename T, typename KeyT>
void cache<T, KeyT>::print_hash_table() const noexcept{
    for (const auto& it : hash_table_) {
        std::cout << it.first << ' ';
    }
    std::cout << '\n';
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

template<typename T, typename KeyT>
void cache<T, KeyT>::insert_item(KeyT key, const T& value, freqIter new_freq_iter) {
    item it(key, value, new_freq_iter);
    (new_freq_iter->freq_list_).push_front(std::move(it));
    hash_table_[key] = (new_freq_iter->freq_list_).begin();
}

template<typename T, typename KeyT>
void cache<T, KeyT>::move_item_element(KeyT key, const T& value, freqIter cache_iter, 
     freqIter pos_to_insert, itemIter pos_to_erase) {
        (cache_iter->freq_list_).erase(pos_to_erase);
        insert_item(key, value, pos_to_insert);
        if ((cache_iter->freq_list_).empty()) {
            cache_.erase(cache_iter);
        }
}


template<typename T, typename KeyT>
bool cache<T, KeyT>::lookup_update(KeyT key, const T& value) {
   /* if (!capacity_) {
        std::cout << "Cache is not useful" << '\n';
        return false;
    }*/

//    print_hash_table();
    print_cache();
    auto is_found = hash_table_.find(key);
    if (is_found == hash_table_.end()) {
        if (is_full()) {    
            //remove last element in the first freq-list node
            auto first_it = cache_.begin();   
            auto key_to_remove = (*std::prev((cache_.begin()->freq_list_).end())).key_;
            hash_table_.erase(key_to_remove);
            (first_it->freq_list_).pop_back();
            if ( (cache_.begin()->freq_list_).empty() ) {
                cache_.pop_front();
            }
            //freq-node with member freq_ = 1 should exist
           /* if ((*first_it).freq_ != 1) {
                cache_.push_front(frequencyItem{1});            
            }*/
            if ( (*cache_.begin()).freq_ != 1 ) {
                cache_.push_front(frequencyItem{1});
            }
            //insert item in front of the freq-list with begin iterator
            insert_item(key, value, cache_.begin());
            //std::cout << "after push ";
        //    printCache();
            return false;
        } /*else if (!cache_size_) {
            //if cache is empty, create freq-list with freq_ = 1
            cache_.push_front(frequencyItem{1});            
        }*/
      //  std::cout << "OTHER KEY = " << key << '\n';
        //if cache has't freq-list with freq_ = 1
        if ((*cache_.begin()).freq_ != 1 || !cache_size_) {
            cache_.push_front(frequencyItem{1});                
        }
        insert_item(key, value, cache_.begin());
        ++cache_size_;
        return false;
    } 
    //std::cout << "key is found = " << key << '\n';
    
    freqIter cache_iter  = (is_found->second)->fr_iter_;
    auto next_cache_iter = std::next(cache_iter);

    if (next_cache_iter != cache_.end() && next_cache_iter->freq_ == (cache_iter->freq_ + 1)) {
        move_item_element(key, value, cache_iter, next_cache_iter, is_found->second);
    } else {
        freqIter inserted_cell = cache_.insert(next_cache_iter, frequencyItem{cache_iter->freq_ + 1}); 
        move_item_element(key, value, cache_iter, inserted_cell, is_found->second);
    }
    return true;
}


size_type check_hits(cache<page_t>& cch, size_type number) {
    size_type hits = 0;
    for (size_type count = 0; count < number; ++count) {
        page_t tmp_page {};
        std::cin >> tmp_page.page_id;
        if (cch.lookup_update(tmp_page.page_id, tmp_page)) {
            ++hits;
        }
    }
    cch.print_cache();
    return hits;
}

}
#endif


