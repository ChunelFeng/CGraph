/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: USemaphore.h
@Time: 2023/10/9 22:01
@Desc: 
***************************/

#ifndef CGRAPH_USEMAPHORE_H
#define CGRAPH_USEMAPHORE_H

CGRAPH_NAMESPACE_BEGIN

#include <mutex>
#include <condition_variable>

#include "../UThreadObject.h"

class USemaphore : public UThreadObject {
public:
    /**
     * 触发一次信号
     */
    CVoid signal() {
        CGRAPH_UNIQUE_LOCK lk(mutex_);
        cnt_++;
        if (cnt_ <= 0) {
            cv_.notify_one();
        }
    }

    /**
     * 等待信号触发
     */
    CVoid wait() {
        CGRAPH_UNIQUE_LOCK lk(mutex_);
        cnt_--;
        if (cnt_ < 0) {
            cv_.wait(lk);
        }
    }

private:
    CInt cnt_ = 0;    // 记录当前的次数
    std::mutex mutex_;
    std::condition_variable cv_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_USEMAPHORE_H
