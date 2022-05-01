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
#include "../ThreadPool/UThreadPoolDefine.h"

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
        if (!is_stop_) {
            return;    // 如果正在执行中，则无法继续执行
        }

        is_stop_ = false;
        /**
         * std::launch::async：在调用async就开始创建线程。
         * std::launch::deferred：延迟加载方式创建线程。调用async时不创建线程，直到调用了future的get或者wait时才创建线程。
         * 后期会考虑将这个功能融合到线程池中去
         */
        future_ = std::async(std::launch::async, [this, interval, task]() {
             while (!is_stop_) {
                 CGRAPH_UNIQUE_LOCK lk(mutex_);
                 const auto& result = cv_.wait_for(lk, std::chrono::milliseconds(interval));
                 if (std::cv_status::timeout == result) {
                     task();
                 }
             }
        });
    }

    /**
     * 关闭定时器
     */
    CVoid stop() {
        if (is_stop_) {
            return;
        }

        is_stop_ = true;
        cv_.notify_one();
        future_.wait();
    }

private:
    std::atomic<CBool> is_stop_ { true };
    std::mutex mutex_;
    std::condition_variable cv_;
    std::future<CVoid> future_ {};
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTIMER_H
