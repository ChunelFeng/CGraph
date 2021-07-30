/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UBarrier.h
@Time: 2021/7/17 1:40 下午
@Desc: 实现线程屏障逻辑，暂未投入使用
***************************/

#ifndef CGRAPH_UBARRIER_H
#define CGRAPH_UBARRIER_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>

#include "../UThreadObject.h"


class UBarrier : public UThreadObject {
public:
    explicit UBarrier(unsigned int cnt)
            : threshold_(cnt), cnt_(cnt) {
    };

    explicit UBarrier() = delete;
    UBarrier(const UBarrier& barrier) = delete;
    UBarrier& operator=(const UBarrier& node) = delete;


    /**
     * 执行等待
     * @return
     */
    bool wait() {
        CGRAPH_UNIQUE_LOCK lock(mutex_);

        unsigned int gen = generation_;
        if (--cnt_ == 0) {
            generation_++;
            cnt_ = threshold_;    // 重置cnt值
            cv_.notify_all();
            status_ = true;
            return true;
        }

        // 添加判断条件，是防止伪唤醒
        cv_.wait(lock, [this, gen] {
            return gen != generation_;
        });

        return false;
    }


    /**
     * 获取执行状态
     * @return
     */
    bool getStatus() {
        CGRAPH_UNIQUE_LOCK lock(mutex_);
        return status_;
    }

private:
    std::mutex mutex_;
    std::condition_variable cv_;
    unsigned int threshold_;                            // 进入次数阈值
    unsigned int cnt_;                                  // 进入次数记录
    unsigned int generation_ {0};                       // 轮数（持续递增，防止伪唤醒）
    bool status_ { false };

    friend class UThreadPool;
};

using UBarrierPtr = UBarrier *;

#endif //CGRAPH_UBARRIER_H
