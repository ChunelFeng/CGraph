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
#include <chrono>

#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T, CUInt capacity = CGRAPH_DEFAULT_RINGBUFFER_SIZE>
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
    UAtomicRingBufferQueue* setCapacity(CUInt size) {
        capacity_ = size;
        ring_buffer_queue_.resize(capacity_);
        return this;
    }

    /**
     * 获取容量信息
     * @return
     */
    CUInt getCapacity() const {
        return capacity_;
    }

    /**
     * 写入信息
     * @tparam TImpl
     * @param value
     * @param strategy
     * @return
     */
    template<class TImpl = T>
    CVoid push(const TImpl& value, URingBufferPushStrategy strategy) {
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            if (isFull()) {
                switch (strategy) {
                    case URingBufferPushStrategy::WAIT:
                        push_cv_.wait(lk, [this] { return !isFull(); });
                        break;
                    case URingBufferPushStrategy::REPLACE:
                        head_ = (head_ + 1) % capacity_;
                        break;
                    case URingBufferPushStrategy::DROP:
                        return;    // 直接返回，不写入即可
                }
            }

            ring_buffer_queue_[tail_] = std::move(c_make_unique<TImpl>(value));
            tail_ = (tail_ + 1) % capacity_;
        }
        pop_cv_.notify_one();
    }

    /**
     * 写入智能指针类型的信息
     * @tparam TImpl
     * @param value
     * @param strategy
     * @return
     */
    template<class TImpl = T>
    CVoid push(std::unique_ptr<TImpl>& value, URingBufferPushStrategy strategy) {
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            if (isFull()) {
                switch (strategy) {
                    case URingBufferPushStrategy::WAIT:
                        push_cv_.wait(lk, [this] { return !isFull(); });
                        break;
                    case URingBufferPushStrategy::REPLACE:
                        head_ = (head_ + 1) % capacity_;
                        break;
                    case URingBufferPushStrategy::DROP:
                        return;    // 直接返回，不写入即可
                }
            }

            ring_buffer_queue_[tail_] = std::move(value);
            tail_ = (tail_ + 1) % capacity_;
        }
        pop_cv_.notify_one();
    }

    /**
     * 等待弹出信息
     * @param value
     * @param timeout
     * @return
     */
    template<class TImpl = T>
    CStatus waitPopWithTimeout(TImpl& value, CMSec timeout) {
        CGRAPH_FUNCTION_BEGIN
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            if (isEmpty()
                && !pop_cv_.wait_for(lk, std::chrono::milliseconds(timeout),
                                     [this] { return !isEmpty(); })) {
                // 如果timeout的时间内，等不到消息，则返回错误信息
                CGRAPH_RETURN_ERROR_STATUS("receive message timeout.")
            }

            value = *ring_buffer_queue_[head_];    // 这里直接进行值copy
            head_ = (head_ + 1) % capacity_;
        }
        push_cv_.notify_one();
        CGRAPH_FUNCTION_END
    }

    /**
     * 等待弹出信息。ps：当入参为智能指针的情况
     * @tparam TImpl
     * @param value
     * @param timeout
     * @return
     */
    template<class TImpl = T>
    CStatus waitPopWithTimeout(std::unique_ptr<TImpl>& value, CMSec timeout) {
        CGRAPH_FUNCTION_BEGIN
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            if (isEmpty()
                && !pop_cv_.wait_for(lk, std::chrono::milliseconds(timeout),
                                     [this] { return !isEmpty(); })) {
                // 如果timeout的时间内，等不到消息，则返回错误信息
                CGRAPH_RETURN_ERROR_STATUS("receive message timeout.")
            }

            /**
             * 当传入的内容，是智能指针的时候，
             * 这里就直接通过 move转移过去好了，跟直接传值的方式，保持区别
             */
            value = std::move(ring_buffer_queue_[head_]);
            head_ = (head_ + 1) % capacity_;
        }
        push_cv_.notify_one();
        CGRAPH_FUNCTION_END
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
    /**
     * 当前队列是否为满
     * @return
     */
    CBool isFull() {
        // 空出来一个位置，这个时候不让 tail写入
        return head_ == (tail_ + 1) % capacity_;
    }

    /**
     * 当前队列是否为空
     * @return
     */
    CBool isEmpty() {
        return head_ == tail_;
    }

    CGRAPH_NO_ALLOWED_COPY(UAtomicRingBufferQueue)

private:
    CUInt head_;                                                    // 头结点位置
    CUInt tail_;                                                    // 尾结点位置
    CUInt capacity_;                                                // 环形缓冲的容量大小

    std::condition_variable push_cv_;                               // 写入的条件变量。为了保持语义完整，也考虑今后多入多出的可能性，不使用 父类中的 cv_了
    std::condition_variable pop_cv_;                                // 读取的条件变量

    std::vector<std::unique_ptr<T> > ring_buffer_queue_;            // 环形缓冲区
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UATOMICRINGBUFFERQUEUE_H
