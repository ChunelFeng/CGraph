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
#include "../UThreadObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class UAtomicQueue : public UThreadObject {
public:
    UAtomicQueue() = default;

    /**
     * 等待弹出
     * @param value
     */
    void waitPop(T& value) {
        CGRAPH_LOCK_GUARD lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });
        value = std::move(*queue_.front());
        queue_.pop();
    }


    /**
     * 尝试弹出
     * @param value
     * @return
     */
    bool tryPop(T& value) {
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
     * @return
     */
    bool tryMultiPop(std::vector<T>& values) {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) {
            return false;
        }

        int i = CGRAPH_MAX_POOL_BATCH_SIZE;
        while (!queue_.empty() && i--) {
            values.emplace_back(std::move(*queue_.front()));
            queue_.pop();
        }

        return true;
    }


    std::unique_ptr<T> waitPop() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });
        std::unique_ptr<T> result = queue_.front();
        queue_.pop();
        return result;
    }


    std::unique_ptr<T> tryPop() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (queue_.empty()) {
            return std::unique_ptr<T>();
        }

        std::unique_ptr<T> res = std::move(queue_.front());
        queue_.pop();
        return res;
    }


    /**
     * 传入数据
     * @param value
     */
    void push(T&& value) {
        std::unique_ptr<T> task(std::make_unique<T>(std::move(value)));
        CGRAPH_LOCK_GUARD lk(mutex_);
        queue_.push(std::move(task));
        cv_.notify_one();
    }


    [[nodiscard]] bool empty() const {
        CGRAPH_LOCK_GUARD lk(mutex_);
        return queue_.empty();
    }

private:
    std::mutex mutex_;
    std::queue<std::unique_ptr<T>> queue_;
    std::condition_variable cv_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UATOMICQUEUE_H
