/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UWorkStealingQueue.h
@Time: 2021/7/2 11:29 下午
@Desc: 
***************************/


#ifndef CGRAPH_UWORKSTEALINGQUEUE_H
#define CGRAPH_UWORKSTEALINGQUEUE_H

#include <queue>
#include <mutex>
#include <deque>

#include "../UThreadObject.h"
#include "../TaskWrapper/UTaskWrapper.h"


class UWorkStealingQueue : public UThreadObject {
public:
    /**
     * 向队列中写入信息
     * @param task
     */
    void push(UTaskWrapper&& task) {
        while (true) {
            if (mutex_.try_lock()) {
                queue_.push_front(std::move(task));
                mutex_.unlock();
                break;
            } else {
                std::this_thread::yield();
            }
        }
    }


    /**
     * 弹出节点，从前部进行
     * @param task
     * @return
     */
    bool tryPop(UTaskWrapper& task) {
        // 这里不使用raii锁，主要是考虑到多线程的情况下，可能会重复进入
        bool result = false;
        if (mutex_.try_lock()) {
            if (!queue_.empty()) {
                task = std::move(queue_.front());    // 从前方弹出
                queue_.pop_front();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 窃取节点，从后方进行
     * @param task
     * @return
     */
    bool trySteal(UTaskWrapper& task) {
        bool result = false;
        if (mutex_.try_lock()) {
            if (!queue_.empty()) {
                task = std::move(queue_.back());    // 从后方窃取
                queue_.pop_back();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }

    /* 禁止直接创建 */
    UWorkStealingQueue() = default;
    UWorkStealingQueue(const UWorkStealingQueue& task) = delete;
    UWorkStealingQueue &operator=(const UWorkStealingQueue& task) = delete;

private:
    std::deque<UTaskWrapper> queue_;
    std::mutex mutex_;
};

#endif //CGRAPH_UWORKSTEALINGQUEUE_H
