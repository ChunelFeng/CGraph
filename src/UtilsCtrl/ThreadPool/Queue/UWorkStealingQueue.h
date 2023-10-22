/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UWorkStealingQueue.h
@Time: 2021/7/2 11:29 下午
@Desc: 实现了一个包含盗取功能的安全队列
***************************/


#ifndef CGRAPH_UWORKSTEALINGQUEUE_H
#define CGRAPH_UWORKSTEALINGQUEUE_H

#include <deque>

#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class UWorkStealingQueue : public UQueueObject {
public:
    /**
     * 向队列中写入信息
     * @param task
     */
    CVoid push(T&& task) {
        while (true) {
            if (lock_.try_lock()) {
                deque_.emplace_front(std::forward<T>(task));
                lock_.unlock();
                break;
            } else {
                std::this_thread::yield();
            }
        }
    }


    /**
     * 尝试往队列里写入信息
     * @param task
     * @return
     */
    CBool tryPush(T&& task) {
        CBool result = false;
        if (lock_.try_lock()) {
            deque_.emplace_back(std::forward<T>(task));
            lock_.unlock();
            result = true;
        }
        return result;
    }


    /**
     * 向队列中写入信息
     * @param task
     */
    CVoid push(std::vector<T>& tasks) {
        while (true) {
            if (lock_.try_lock()) {
                for (const auto& task : tasks) {
                    deque_.emplace_front(std::forward<T>(task));
                }
                lock_.unlock();
                break;
            } else {
                std::this_thread::yield();
            }
        }
    }


    /**
     * 尝试批量写入内容
     * @param tasks
     * @return
     */
    CBool tryPush(std::vector<T>& tasks) {
        CBool result = false;
        if (lock_.try_lock()) {
            for (const auto& task : tasks) {
                deque_.emplace_back(std::forward<T>(task));
            }
            lock_.unlock();
            result = true;
        }
        return result;
    }


    /**
     * 弹出节点，从头部进行
     * @param task
     * @return
     */
    CBool tryPop(T& task) {
        // 这里不使用raii锁，主要是考虑到多线程的情况下，可能会重复进入
        bool result = false;
        if (!deque_.empty() && lock_.try_lock()) {
            if (!deque_.empty()) {
                task = std::forward<T>(deque_.front());    // 从前方弹出
                deque_.pop_front();
                result = true;
            }
            lock_.unlock();
        }

        return result;
    }


    /**
     * 从头部开始批量获取可执行任务信息
     * @param taskArr
     * @param maxLocalBatchSize
     * @return
     */
    CBool tryPop(std::vector<T>& taskArr, int maxLocalBatchSize) {
        bool result = false;
        if (!deque_.empty() && lock_.try_lock()) {
            while (!deque_.empty() && maxLocalBatchSize--) {
                taskArr.emplace_back(std::forward<T>(deque_.front()));
                deque_.pop_front();
                result = true;
            }
            lock_.unlock();
        }

        return result;
    }


    /**
     * 窃取节点，从尾部进行
     * @param task
     * @return
     */
    CBool trySteal(T& task) {
        bool result = false;
        if (!deque_.empty() && lock_.try_lock()) {
            if (!deque_.empty()) {
                task = std::forward<T>(deque_.back());    // 从后方窃取
                deque_.pop_back();
                result = true;
            }
            lock_.unlock();
        }

        return result;
    }


    /**
     * 批量窃取节点，从尾部进行
     * @param taskArr
     * @return
     */
    CBool trySteal(std::vector<T>& taskArr, int maxStealBatchSize) {
        bool result = false;
        if (!deque_.empty() && lock_.try_lock()) {
            while (!deque_.empty() && maxStealBatchSize--) {
                taskArr.emplace_back(std::forward<T>(deque_.back()));
                deque_.pop_back();
                result = true;
            }
            lock_.unlock();
        }

        return result;    // 如果非空，表示盗取成功
    }

    UWorkStealingQueue() = default;

    CGRAPH_NO_ALLOWED_COPY(UWorkStealingQueue)

private:
    std::deque<T> deque_;            // 存放任务的双向队列
    std::mutex lock_;                // 用于处理deque_的锁
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UWORKSTEALINGQUEUE_H
