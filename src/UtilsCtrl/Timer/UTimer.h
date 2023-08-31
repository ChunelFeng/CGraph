/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTimer.h
@Time: 2021/12/31 8:34 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTIMER_H
#define CGRAPH_UTIMER_H

#include "../UtilsObject.h"

CGRAPH_NAMESPACE_BEGIN

class UTimer : public UtilsObject {
public:
    UTimer() = default;
    ~UTimer() override {
        stop();
    }

    /**
     * 开始执行定时器
     * @param interval 间隔时间，单位ms
     * @param task
     */
    template<typename FunctionType>
    CVoid start(CMSec interval, const FunctionType& task) {
        if (!is_stop_.exchange(false)) {
            return;    // 如果正在执行中，则无法继续执行
        }

        left_interval_ = interval;
        origin_interval_ = interval;

        /**
         * std::launch::async：在调用async就开始创建线程。
         * std::launch::deferred：延迟加载方式创建线程。调用async时不创建线程，直到调用了future的get或者wait时才创建线程。
         */
        future_ = std::async(std::launch::async, [this, task]() {
             while (!is_stop_) {
                 CGRAPH_UNIQUE_LOCK lk(mutex_);
                 auto result = cv_.wait_for(lk, std::chrono::milliseconds(left_interval_));
                 if (std::cv_status::timeout == result && !is_stop_) {
                     CMSec start = CGRAPH_GET_CURRENT_MS();
                     task();
                     CMSec span = CGRAPH_GET_CURRENT_MS() - start;
                     /**
                      * 如果任务执行时间 < 设定的时间，则消除任务耗时影响
                      * 如果任务执行时间 > 设定的时间，则继续sleep设定时长
                      */
                     left_interval_ = (origin_interval_ > span)
                             ? (origin_interval_ - span)
                             : (origin_interval_);
                 }
             }
        });
    }

    /**
     * 关闭定时器
     */
    CVoid stop() {
        if (is_stop_.exchange(true)) {
            return;
        }

        cv_.notify_one();
        future_.wait();
    }

private:
    std::atomic<CBool> is_stop_ { true };
    std::mutex mutex_;
    std::condition_variable cv_;
    std::future<CVoid> future_ {};
    CMSec origin_interval_ = 0;                 // 设定的耗时信息
    CMSec left_interval_ = 0;                   // 除去task运行的时间，还剩下的时间
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTIMER_H
