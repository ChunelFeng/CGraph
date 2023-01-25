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
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) {
            return false;
        }
        value = std::move(*queue_.front());
        queue_.pop();
        return true;
    }


    /**
     * 尝试弹出多个任务
     * @param values
     * @param maxPoolBatchSize
     * @return
     */
    CBool tryPop(std::vector<T>& values, int maxPoolBatchSize) {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty() || maxPoolBatchSize <= 0) {
            return false;
        }

        while (!queue_.empty() && maxPoolBatchSize--) {
            values.emplace_back(std::move(*queue_.front()));
            queue_.pop();
        }

        return true;
    }


    /**
     * 阻塞式等待弹出
     * @return
     */
    std::unique_ptr<T> waitPop() {
        CGRAPH_UNIQUE_LOCK lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });
        std::unique_ptr<T> result = std::move(queue_.front());
        queue_.pop();
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
        std::unique_ptr<T> task(c_make_unique<T>(std::move(value)));
        CGRAPH_LOCK_GUARD lk(mutex_);
        queue_.push(std::move(task));
        cv_.notify_one();
    }


    /**
     * 判定队列是否为空
     * @return
     */
    [[nodiscard]] CBool empty() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        return queue_.empty();
    }

    CGRAPH_NO_ALLOWED_COPY(UAtomicQueue)

private:
    std::queue<std::unique_ptr<T>> queue_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UATOMICQUEUE_H
