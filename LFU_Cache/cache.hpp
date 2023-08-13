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
public:
    explicit cache(size_type capacity);
    ~cache() = default;

    bool isFull() const noexcept;
    bool lookup_update(KeyT key, const T& value);
    void printCache() const noexcept;
private:
    size_type cache_size_; 
    size_type capacity_;
    std::unordered_map<KeyT, itemIter> hash_table_;
    std::list<frequencyItem> cache_;

    struct item {
        KeyT key_;
        T value_;
        freqIter fr_iter_;

        item(KeyT key, const T& value):
            key_{key}, value_{value} {};
        
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
bool cache<T, KeyT>::isFull() const noexcept {
    return cache_size_ == capacity_;
}

template<typename T, typename KeyT>
void cache<T, KeyT>::insert_item(KeyT key, const T& value, freqIter new_freq_iter) {
    item it(key, value, new_freq_iter);
    (*new_freq_iter).freq_list_.push_front(std::move(it));
    hash_table_[key] = (new_freq_iter->freq_list_).begin();
}

template<typename T, typename KeyT>
void cache<T, KeyT>::printCache() const noexcept{
    std::cout << "cache: ";
    for (const auto& it1 : cache_) {
        std::cout << it1.freq_ << ":";
        for (const auto& it2 : it1.freq_list_) {
            std::cout << it2.key_ << ' ';
        }
        std::cout << '\t';
    }
    std::cout << std::endl;
}

template<typename T, typename KeyT>
bool cache<T, KeyT>::lookup_update(KeyT key, const T& value) {
    if (!capacity_) {
        std::cout << "Cache is not useful" << '\n';
        return false;
    }
    auto is_found = hash_table_.find(key);
    printCache();
    if (is_found == hash_table_.end()) {
        if (isFull()) {    
            //remove last element in the first freq-list node
            std::cout << "CACHE IS FULL\n";
            auto first_it = cache_.begin();   
            auto key_to_remove = (*std::prev(first_it->freq_list_.end())).key_;
            hash_table_.erase(key_to_remove);
            (first_it->freq_list_).pop_back();
            std::cout << "after pop ";
            printCache();
            //freq-node with member freq_ = 1 should exist
            if ((*first_it).freq_ != 1) {
                cache_.push_front(frequencyItem{1});            
            }
            //insert item in front of the freq-list with begin iterator
            insert_item(key, value, cache_.begin());
            std::cout << "after push ";
            printCache();
            return false;
        } else if (!cache_size_) {
            std::cout << "zero size key = " << key << '\n';
            //if cache is empty, create freq-list with freq_ = 1
            cache_.push_front(frequencyItem{1});            
        }
        std::cout << "other key = " << key << '\n';
        //if cache has't    freq-list with freq_ = 1
        if ((*cache_.begin()).freq_ != 1) {
            cache_.push_front(frequencyItem{1});                
        }
        insert_item(key, value, cache_.begin());
        ++cache_size_;
        return false;
    } 
    std::cout << "key is found = " << key << '\n';
    
    freqIter cache_iter  = (is_found->second)->fr_iter_;
    auto next_cache_iter = std::next(cache_iter);
    if (next_cache_iter == cache_.end()) {
        cache_.push_back(frequencyItem{cache_iter->freq_ + 1});
        auto save_iter = cache_iter;
        (cache_iter->freq_list_).erase(is_found->second);
        insert_item(key, value, ++cache_iter);
        if ((save_iter->freq_list_).empty()) {
            cache_.erase(save_iter);
        }
    } else if (next_cache_iter->freq_ == (cache_iter->freq_ + 1)) {
        std::cout << "KEEEEY = " << key << std::endl;
        std::cout << "FREEQ: " << next_cache_iter->freq_ << std::endl;
        for (auto& it : next_cache_iter->freq_list_) {
            std::cout << it.key_ << ' ';
        }
        std::cout << '\n';
        (cache_iter->freq_list_).erase(is_found->second);
        insert_item(key, value, next_cache_iter);
        printCache();
        std::cout << "ERAAASEEE\n";
        printCache();
        if ((cache_iter->freq_list_).empty()) {
            cache_.erase(cache_iter);
         }
    } else {
        auto inserted_cell = cache_.insert(next_cache_iter, frequencyItem{cache_iter->freq_ + 1}); 
        (cache_iter->freq_list_).erase(is_found->second);
        insert_item(key, value, inserted_cell);
        if ((cache_iter->freq_list_).empty()) {
            cache_.erase(cache_iter);
        }
    }
    return true;
}

size_type CheckHits(cache<page_t>& cch, size_type number) {
    size_type hits = 0;
    for (size_type count = 0; count < number; ++count) {
        page_t tmp_page {};
        std::cin >> tmp_page.page_id;
        if (cch.lookup_update(tmp_page.page_id, tmp_page)) {
            ++hits;
        }
    }
    cch.printCache();
    return hits;
}

}
#endif


