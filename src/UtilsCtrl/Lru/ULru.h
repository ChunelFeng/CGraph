/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: ULru.h
@Time: 2021/9/18 11:55 下午
@Desc: 
***************************/

#ifndef CGRAPH_ULRU_H
#define CGRAPH_ULRU_H

#include <list>
#include <unordered_map>

#include "../UtilsObject.h"
#include "ULruNode.h"

CGRAPH_NAMESPACE_BEGIN

const static CSize DEFAULT_LRU_CACHE_CAPACITY = 10;

template<typename K, typename V, CSize N = DEFAULT_LRU_CACHE_CAPACITY>
class ULru : public UtilsObject {
public:
    explicit ULru() {
        capacity_ = N;
    }

    ~ULru() override {
        clear();
    }

    /**
     * 信息插入Lru
     * @param key
     * @param value
     */
    CVoid put(const K& key, const V& value) {
        auto cur = cache_.find(key);
        if (cur != cache_.end()) {
            /* 如果在cache中找到，则删除（稍后更新至最前） */
            nodes_.erase(cache_[key]);
            cache_.erase(key);
            cur_size_--;
        } else if (cur_size_ >= capacity_) {
            /* 如果超限，则删除最后一个节点信息 */
            auto back = nodes_.back();
            cache_.erase(back.key_);
            nodes_.pop_back();
            cur_size_--;
        }

        nodes_.emplace_front(ULruNode<K, V>(key, value));
        cache_[key] = nodes_.begin();
        cur_size_++;
    }

    /**
     * 从Lru中获取节点信息
     * @param key
     * @return
     */
    V get(const K& key) {
        V result;
        auto cur = cache_.find(key);
        if (cur != cache_.end()) {
            result = cur->second->value_;
            put(key, result);
        }

        return result;
    }

    /**
     * 设置capacity信息
     * @param capacity
     */
    CVoid setCapacity(CSize capacity) {
        this->capacity_ = capacity;
    }

    /**
     * 获取capacity信息
     * @return
     */
    CSize getCapacity() {
        return this->capacity_;
    }

    /**
     * 清空Lru中的信息
     */
    CVoid clear() {
        nodes_.clear();
        cache_.clear();
        cur_size_ = 0;
    }


private:
    CSize cur_size_ { 0 };                                                           // 当前尺寸
    CSize capacity_ { DEFAULT_LRU_CACHE_CAPACITY };                                  // 最大容量
    std::unordered_map<K, typename std::list<ULruNode<K, V>>::iterator> cache_;      // 缓存信息
    std::list<ULruNode<K, V>> nodes_;                                                // 节点链表
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_ULRU_H
