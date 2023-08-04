/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UAtomicQueue.h
@Time: 2021/7/2 11:28 下午
@Desc: 设计了一个安全队列
***************************/

#ifndef CGRAPH_UATOMICQUEUE_H
#define CGRAPH_UATOMICQUEUE_H

#include <memory>
#include <mutex>
#include <queue>
#include <condition_variable>

#include "../UThreadPoolDefine.h"
#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class UAtomicQueue : public UQueueObject {
public:
    UAtomicQueue() = default;

    /**
     * 等待弹出
     * @param value
     */
    CVoid waitPop(T& value) {
        CGRAPH_UNIQUE_LOCK lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });
        value = std::move(*queue_.front());
        queue_.pop();
    }


    /**
     * 尝试弹出
     * @param value
     * @return
     */
    CBool tryPop(T& value) {
        CBool result = false;
        if (mutex_.try_lock()) {
            if (!queue_.empty()) {
                value = std::move(*queue_.front());
                queue_.pop();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 尝试弹出多个任务
     * @param values
     * @param maxPoolBatchSize
     * @return
     */
    CBool tryPop(std::vector<T>& values, int maxPoolBatchSize) {
        CBool result = false;
        if (mutex_.try_lock()) {
            while (!queue_.empty() && maxPoolBatchSize-- > 0) {
                values.emplace_back(std::move(*queue_.front()));
                queue_.pop();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 阻塞式等待弹出
     * @return
     */
    std::unique_ptr<T> popWithTimeout(CMSec ms) {
        CGRAPH_UNIQUE_LOCK lk(mutex_);
        if (!cv_.wait_for(lk, std::chrono::milliseconds(ms), [this] { return !queue_.empty(); })) {
            return nullptr;
        }

        std::unique_ptr<T> result = std::move(queue_.front());
        queue_.pop();    // 如果等成功了，则弹出一个信息
        return result;
    }


    /**
     * 非阻塞式等待弹出
     * @return
     */
    std::unique_ptr<T> tryPop() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) { return std::unique_ptr<T>(); }
        std::unique_ptr<T> ptr = std::move(queue_.front());
        queue_.pop();
        return ptr;
    }


    /**
     * 传入数据
     * @param value
     */
    CVoid push(T&& value) {
        std::unique_ptr<typename std::remove_reference<T>::type>     \
                task(c_make_unique<typename std::remove_reference<T>::type>(std::forward<T>(value)));
        while (true) {
            if (mutex_.try_lock()) {
                queue_.push(std::move(task));
                mutex_.unlock();
                break;
            } else {
                std::this_thread::yield();
            }
        }
        cv_.notify_one();
    }


    /**
     * 判定队列是否为空
     * @return
     */
    CBool empty() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        return queue_.empty();
    }

    CGRAPH_NO_ALLOWED_COPY(UAtomicQueue)

private:
    std::queue<std::unique_ptr<T>> queue_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UATOMICQUEUE_H
