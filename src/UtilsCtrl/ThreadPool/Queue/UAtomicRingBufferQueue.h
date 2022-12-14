/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UAtomicRingBufferQueue.h
@Time: 2022/10/22 22:32
@Desc: 本 queue 仅支持单入单出模式
***************************/

#ifndef CGRAPH_UATOMICRINGBUFFERQUEUE_H
#define CGRAPH_UATOMICRINGBUFFERQUEUE_H

#include <vector>
#include <atomic>

#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T, CUint capacity = CGRAPH_DEFAULT_RINGBUFFER_SIZE>
class UAtomicRingBufferQueue : public UQueueObject {
public:
    explicit UAtomicRingBufferQueue() {
        head_ = 0;
        tail_ = 0;
        capacity_ = capacity;
        ring_buffer_queue_.resize(capacity_);
    }

    ~UAtomicRingBufferQueue() override {
        clear();
    }

    /**
     * 设置容量信息
     * @param size
     * @return
     * @notice 谨慎使用，push信息之后，不推荐使用
     */
    UAtomicRingBufferQueue* setCapacity(CUint size) {
        capacity_ = size;
        ring_buffer_queue_.resize(capacity_);
        return this;
    }

    /**
     * 获取容量信息
     * @return
     */
    [[nodiscard]] CUint getCapacity() const {
        return capacity_;
    }

    /**
     * 写入信息
     * @param value
     * @return
     */
    template<class TImpl = T>
    CVoid push(const TImpl& value) {
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            if (isFull()) {
                push_cv_.wait(lk, [this] { return !isFull(); });
            }

            ring_buffer_queue_[tail_] = std::move(std::make_unique<TImpl>(value));
            tail_ = (tail_ + 1) % capacity_;
        }
        pop_cv_.notify_one();
    }

    /**
     * 等待弹出信息
     * @param value
     * @return
     */
    template<class TImpl = T>
    CVoid waitPop(TImpl& value) {
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            if (isEmpty()) {
                pop_cv_.wait(lk, [this] { return !isEmpty(); });
            }

            value = (*ring_buffer_queue_[head_]);
            *ring_buffer_queue_[head_] = {};
            head_ = (head_ + 1) % capacity_;
        }
        push_cv_.notify_one();
    }

    /**
     * 清空所有的数据
     * @return
     */
    CStatus clear() {
        CGRAPH_FUNCTION_BEGIN
        ring_buffer_queue_.resize(0);
        head_ = 0;
        tail_ = 0;
        CGRAPH_FUNCTION_END
    }

protected:
    CBool isFull() {
        // 空出来一个位置，这个时候不让 tail写入
        return head_ == (tail_ + 1) % capacity_;
    }

    CBool isEmpty() {
        return head_ == tail_;
    }

    CGRAPH_NO_ALLOWED_COPY(UAtomicRingBufferQueue)

private:
    CUint head_;                                                    // 头结点位置
    CUint tail_;                                                    // 尾结点位置
    CUint capacity_;                                                // 环形缓冲的容量大小

    std::condition_variable push_cv_;                               // 写入的条件变量。为了保持语义完整，也考虑今后多入多出的可能性，不使用 父类中的 cv_了
    std::condition_variable pop_cv_;                                // 读取的条件变量

    std::vector<std::unique_ptr<T> > ring_buffer_queue_;            // 环形缓冲区
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UATOMICRINGBUFFERQUEUE_H
