/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UWorkStealingQueue.h
@Time: 2021/7/2 11:29 下午
@Desc: 实现了一个包含盗取功能的安全队列
***************************/


#ifndef CGRAPH_UWORKSTEALINGQUEUE_H
#define CGRAPH_UWORKSTEALINGQUEUE_H

#include <queue>
#include <mutex>
#include <deque>

#include "UQueueObject.h"
#include "../Task/UTask.h"

CGRAPH_NAMESPACE_BEGIN

class UWorkStealingQueue : public UQueueObject {
public:
    UWorkStealingQueue() = default;

    /**
     * 向队列中写入信息
     * @param task
     */
    CVoid push(UTask&& task) {
        while (true) {
            if (mutex_.try_lock()) {
                deque_.emplace_front(std::move(task));
                mutex_.unlock();
                break;
            } else {
                std::this_thread::yield();
            }
        }
    }


    /**
     * 弹出节点，从头部进行
     * @param task
     * @return
     */
    CBool tryPop(UTask& task) {
        // 这里不使用raii锁，主要是考虑到多线程的情况下，可能会重复进入
        bool result = false;
        if (mutex_.try_lock()) {
            if (!deque_.empty()) {
                task = std::move(deque_.front());    // 从前方弹出
                deque_.pop_front();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 从头部开始批量获取可执行任务信息
     * @param taskArr
     * @param maxLocalBatchSize
     * @return
     */
    CBool tryPop(UTaskArrRef taskArr,
                 int maxLocalBatchSize) {
        bool result = false;
        if (mutex_.try_lock()) {
            while (!deque_.empty() && maxLocalBatchSize--) {
                taskArr.emplace_back(std::move(deque_.front()));
                deque_.pop_front();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 窃取节点，从尾部进行
     * @param task
     * @return
     */
    CBool trySteal(UTask& task) {
        bool result = false;
        if (mutex_.try_lock()) {
            if (!deque_.empty()) {
                task = std::move(deque_.back());    // 从后方窃取
                deque_.pop_back();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 批量窃取节点，从尾部进行
     * @param taskArr
     * @return
     */
    CBool trySteal(UTaskArrRef taskArr, int maxStealBatchSize) {
        bool result = false;
        if (mutex_.try_lock()) {
            while (!deque_.empty() && maxStealBatchSize--) {
                taskArr.emplace_back(std::move(deque_.back()));
                deque_.pop_back();
                result = true;
            }
            mutex_.unlock();
        }

        return result;    // 如果非空，表示盗取成功
    }

    CGRAPH_NO_ALLOWED_COPY(UWorkStealingQueue)

private:
    std::deque<UTask> deque_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UWORKSTEALINGQUEUE_H
