#ifndef LFU_CACHE_
#define LFU_CACHE_

#include <iostream>
#include <unordered_map>
#include <iterator>
#include <list>

//|----------------------------|//
//|    LFU cache algorithm     |//
//|----------------------------|//

namespace yLAB { 

template<typename T, typename KeyT = int>
class lfu_cache {
public:
    using size_type = std::size_t;  
    using page_t    = std::pair<T, KeyT>;
private:
    static constexpr size_type MIN_CAPACITY = 1;

    struct item;
    struct frequencyItem;
    
    using freqIter = typename std::list<frequencyItem>::iterator;
    using itemIter = typename std::list<item>::iterator;  
    
    void insert_item(const page_t& value, freqIter iter);
    void remove_last_item();
public:
    explicit lfu_cache(size_type capacity):
                    capacity_{std::max(MIN_CAPACITY, capacity)} {};
    ~lfu_cache() = default;

    bool is_full() const noexcept;
template<typename F>
    bool lookup_update(KeyT key, F get_page);
#ifdef DEBUG
    void print_cache() const;
#endif
private:
    const size_type capacity_;
    std::unordered_map<KeyT, itemIter> hash_table_;
    std::list<frequencyItem> cache_;

    struct item {
        page_t value_;
        freqIter fr_iter_;
        //constructor for list emplace() method    
        item(const page_t& value, freqIter it):
            value_{value},
            fr_iter_{it} {};
    };
    
    struct frequencyItem {
        const size_type freq_;   
        std::list<item> freq_list_;

        explicit frequencyItem(size_type freq):
            freq_{freq} {};
    };
}; // <-- class lfu_cache

template<typename T, typename KeyT>
template<typename F>
bool lfu_cache<T, KeyT>::lookup_update(KeyT key, F get_page) {
    auto hash_iter = hash_table_.find(key);
    if (hash_iter == hash_table_.end()) {
        if (is_full()) {    
            remove_last_item();
            //freq-node with member freq_ = 1 must exist because we got new element
            if (cache_.begin()->freq_ != 1) {
                cache_.emplace_front(1);
            }
        } else {
            //if cache has't freq-list with freq_ == 1
            if (cache_.begin()->freq_ != 1 || hash_table_.empty()) {
                cache_.emplace_front(1);                
            }
        }
        insert_item(get_page(key), cache_.begin());
        return false;
    } 
   //element was found in hash_table //
   //get Iter of list with found element
    freqIter cache_iter  = hash_iter->second->fr_iter_;
    auto next_cache_iter = std::next(cache_iter);
    cache_iter->freq_list_.erase(hash_iter->second);//erase item on found key
    //looking for next suitable frequencyItem: if next one isn't suitable than insert your own
    if (next_cache_iter != cache_.end() && next_cache_iter->freq_ == (cache_iter->freq_ + 1)) {
        insert_item(get_page(key), next_cache_iter);
    } else {
        freqIter inserted_cell = cache_.emplace(next_cache_iter, cache_iter->freq_ + 1); 
        insert_item(get_page(key), inserted_cell);
    }
    if (cache_iter->freq_list_.empty()) {
        cache_.erase(cache_iter);
    }
    return true;
}

template<typename T, typename KeyT>
bool lfu_cache<T, KeyT>::is_full() const noexcept {
    return hash_table_.size() == capacity_;
}

template<typename T, typename KeyT>
void lfu_cache<T, KeyT>::insert_item(const page_t& value, freqIter new_freq_iter) {
    auto& new_freq_list = new_freq_iter->freq_list_;
    new_freq_list.emplace_front(value, new_freq_iter);  //push new item in the frequencyItem-list                        
    hash_table_[value.second] = new_freq_list.begin(); //saving new item iter                                                            
}

template<typename T, typename KeyT>
void lfu_cache<T, KeyT>::remove_last_item() {
    //remove last element in the first freq-list node
    auto& freq_list = cache_.begin()->freq_list_;
    auto key_to_remove = std::prev(freq_list.end())->value_.second;
    hash_table_.erase(key_to_remove);
    freq_list.pop_back();
    if (freq_list.empty()) {
        cache_.pop_front();
    }
}

#ifdef DEBUG
template<typename T, typename KeyT>
void lfu_cache<T, KeyT>::print_cache() const {
    std::cout << "cache:\n";
    for (const auto& freq_item : cache_) {
        std::cout << freq_item.freq_ << ":";
        for (const auto& list_elem : freq_item.freq_list_) {
            std::cout << list_elem.value.second << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "\n\n";
}
#endif

} // <-- namespace yLab

#endif

