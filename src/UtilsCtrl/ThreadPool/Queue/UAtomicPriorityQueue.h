/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UAtomicPriorityQueue.h
@Time: 2022/10/1 21:40
@Desc: 线程安全的优先队列。因为 priority_queue和queue的弹出方式不一致，故暂时不做合并
***************************/

#ifndef CGRAPH_UATOMICPRIORITYQUEUE_H
#define CGRAPH_UATOMICPRIORITYQUEUE_H

#include <queue>

#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class UAtomicPriorityQueue : public UQueueObject {
public:
    UAtomicPriorityQueue() = default;

    /**
     * 尝试弹出
     * @param value
     * @return
     */
    CBool tryPop(T& value) {
        CGRAPH_LOCK_GUARD lk(mutex_);
        if (priority_queue_.empty()) {
            return false;
        }
        value = std::move(*priority_queue_.top());
        priority_queue_.pop();
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
        if (priority_queue_.empty() || maxPoolBatchSize <= 0) {
            return false;
        }

        while (!priority_queue_.empty() && maxPoolBatchSize--) {
            values.emplace_back(std::move(*priority_queue_.top()));
            priority_queue_.pop();
        }

        return true;
    }


    /**
     * 传入数据
     * @param value
     * @param priority 任务优先级，数字排序
     * @return
     */
    CVoid push(T&& value, int priority) {
        std::unique_ptr<T> task(c_make_unique<T>(std::move(value), priority));
        CGRAPH_LOCK_GUARD lk(mutex_);
        priority_queue_.push(std::move(task));
    }


    /**
     * 判定队列是否为空
     * @return
     */
    [[nodiscard]] CBool empty() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        return priority_queue_.empty();
    }

    CGRAPH_NO_ALLOWED_COPY(UAtomicPriorityQueue)

private:
    std::priority_queue<std::unique_ptr<T> > priority_queue_;    // 优先队列信息，根据重要级别决定先后执行顺序
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UATOMICPRIORITYQUEUE_H
