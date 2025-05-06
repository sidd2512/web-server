#include <list>
#include <unordered_map>
#include <string>
#include <mutex>
#include <optional>


template<typename Key, typename Value>
class ThreadSafeLRUCache {
private:
    size_t capacity;
    std::list<std::pair<Key, Value>> cache_list;
    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> cache_map;
    mutable std::mutex mutex;

public:
    ThreadSafeLRUCache(size_t cap) : capacity(cap) {}

    std::optional<Value> get(const Key& key) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = cache_map.find(key);
        if (it == cache_map.end()) return std::nullopt;
        
        cache_list.splice(cache_list.begin(), cache_list, it->second);
        return it->second->second;
    }

    void put(const Key& key, const Value& value) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = cache_map.find(key);
        if (it != cache_map.end()) {
            cache_list.erase(it->second);
        }
        
        cache_list.push_front({key, value});
        cache_map[key] = cache_list.begin();
        
        if (cache_map.size() > capacity) {
            auto last = cache_list.end();
            last--;
            cache_map.erase(last->first);
            cache_list.pop_back();
        }
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex);
        return cache_map.size();
    }
};