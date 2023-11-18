#include <iostream>
#include <vector>
#include <list>
#include <unordered_set>
#include <random>
#include <ctime>
#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>


class weak_perfect final {
    static constexpr u_int MIN_CAPACITY = 1;

    using Key       = u_int;
    using cacheIter = typename std::vector<Key>::iterator;

    cacheIter find(Key key);
    bool is_full() const noexcept;
    cacheIter find_farthest_value(cacheIter buff_it);
public:
template<typename Iter>
    weak_perfect(u_int capacity, Iter begin, Iter end);

    void lookup_update();
    u_int get_hits() const noexcept;
private:
    const u_int capacity_;
    std::vector<Key> buffer_;
    std::vector<Key> cache_;
    u_int hits_;
};

template<typename Iter>
weak_perfect::weak_perfect(u_int capacity, Iter begin, Iter end):
               capacity_{std::max(MIN_CAPACITY, capacity)},
               buffer_{begin, end},
               hits_{0} {
    cache_.reserve(capacity_);
}

void weak_perfect::lookup_update() {
    for (auto buff_it = buffer_.begin(); buff_it != buffer_.end(); ++buff_it) {
        auto pos_it = find(*buff_it);
        if (pos_it == cache_.end()) {
            if (std::find(std::next(buff_it), buffer_.end(), *buff_it) == buffer_.end()) {
                continue;
            }
            if (is_full()) {
                auto most_far = find_farthest_value(buff_it);
                *most_far = *buff_it;
            } else {
                cache_.push_back(*buff_it);
            }
        } else {
            ++hits_;
        }
    }
}

bool weak_perfect::is_full() const noexcept {
    return cache_.size() == capacity_;
}

weak_perfect::cacheIter weak_perfect::find_farthest_value(cacheIter buff_it) {
    std::vector<cacheIter> distance_iters{};
    for (cacheIter it1 = cache_.begin(); it1 != cache_.end(); ++it1) {
        auto key_iter = std::find(buff_it, buffer_.end(), *it1);
        if (key_iter == buffer_.end()) {
            return it1;
        }
        distance_iters.push_back(key_iter);
    }
    if (distance_iters.empty()) {
        return cache_.begin();
    }
    std::sort(distance_iters.begin(), distance_iters.end(), std::greater<cacheIter>());
    Key key = *distance_iters.front();
    return std::find(cache_.begin(), cache_.end(), key);
}

weak_perfect::cacheIter weak_perfect::find(Key key) {
    return std::find(cache_.begin(), cache_.end(), key);
}

u_int weak_perfect::get_hits() const noexcept {
    return hits_;
}
int main() {
    
    std::size_t cap{};
    std::size_t size{};

    std::cin >> cap >> size;
    
    std::vector<std::size_t> data{};
    data.reserve(size);
    for (std::size_t i = 0; i < size; ++i) {
        std::size_t tmp{};
        std::cin >> tmp;
        data.push_back(tmp);
    }

    weak_perfect cache(cap, data.begin(), data.end());
    cache.lookup_update();
    std::cout << cache.get_hits() << std::endl;


}
