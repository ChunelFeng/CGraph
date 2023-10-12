/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: ULockFreeRingBufferQueue.h
@Time: 2023/10/7 21:35
@Desc: 
***************************/

#ifndef CGRAPH_ULOCKFREERINGBUFFERQUEUE_H
#define CGRAPH_ULOCKFREERINGBUFFERQUEUE_H

#include <atomic>
#include <memory>

#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T, CInt CAPACITY = 32>
class ULockFreeRingBufferQueue : public UQueueObject {
public:
    explicit ULockFreeRingBufferQueue() {
        head_ = 0;
        tail_ = 0;
        ring_buffer_.resize(CAPACITY);
    }

    ~ULockFreeRingBufferQueue() override {
        ring_buffer_.clear();
    }

    /**
     * 写入一个任务
     * @param value
     */
    CVoid push(T&& value) {
        int curTail = tail_.load(std::memory_order_relaxed);
        int nextTail = (curTail + 1) % CAPACITY;

        while (nextTail == head_.load(std::memory_order_acquire)) {
            // 队列已满，等待其他线程出队
            std::this_thread::yield();
        }

        ring_buffer_[curTail] = std::move(value);
        tail_.store(nextTail, std::memory_order_release);
    }

    /**
     * 尝试弹出一个任务
     * @param value
     * @return
     */
    CBool tryPop(T& value) {
        int curHead = head_.load(std::memory_order_relaxed);
        if (curHead == tail_.load(std::memory_order_acquire)) {
            // 队列已空，直接返回false
            return false;
        }

        value = std::move(ring_buffer_[curHead]);

        int nextHead = (curHead + 1) % CAPACITY;
        head_.store(nextHead, std::memory_order_release);
        return true;
    }


private:
    std::atomic<CInt> head_;                                // 开始元素（较早写入的）的位置
    std::atomic<CInt> tail_;                                // 尾部的位置
    std::vector<std::unique_ptr<T> > ring_buffer_;          // 环形队列
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_ULOCKFREERINGBUFFERQUEUE_H
