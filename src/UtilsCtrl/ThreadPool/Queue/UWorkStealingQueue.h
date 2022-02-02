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
#include "../Task/UTaskWrapper.h"

CGRAPH_NAMESPACE_BEGIN

class UWorkStealingQueue : public UThreadObject {
public:
    UWorkStealingQueue() = default;

    /**
     * 向队列中写入信息
     * @param task
     */
    void push(UTaskWrapper&& task) {
        while (true) {
            if (mutex_.try_lock()) {
                queue_.emplace_front(std::move(task));
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
     * 从头部开始批量获取可执行任务信息
     * @param taskArr
     * @param maxLocalBatchSize
     * @return
     */
    bool tryPop(UTaskWrapperArrRef taskArr,
                int maxLocalBatchSize) {
        bool result = false;
        if (mutex_.try_lock()) {
            while (!queue_.empty() && maxLocalBatchSize--) {
                taskArr.emplace_back(std::move(queue_.front()));
                queue_.pop_front();
                result = true;
            }
            mutex_.unlock();
        }

        return result;    // 如果弹出成功，则arr非空
    }


    /**
     * 窃取节点，从尾部进行
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


    /**
     * 批量窃取节点，从尾部进行
     * @param taskArr
     * @return
     */
    bool trySteal(UTaskWrapperArrRef taskArr, int maxStealBatchSize) {
        bool result = false;
        if (mutex_.try_lock()) {
            while (!queue_.empty() && maxStealBatchSize--) {
                taskArr.emplace_back(std::move(queue_.back()));
                queue_.pop_back();
                result = true;
            }
            mutex_.unlock();
        }

        return result;    // 如果非空，表示盗取成功
    }

    /** 直接禁止copy和赋值 */
    UWorkStealingQueue(const UWorkStealingQueue& task) = delete;
    UWorkStealingQueue &operator=(const UWorkStealingQueue& task) = delete;

private:
    std::deque<UTaskWrapper> queue_;
    std::mutex mutex_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UWORKSTEALINGQUEUE_H
