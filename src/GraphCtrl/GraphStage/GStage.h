/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStage.h
@Time: 2024/12/13 19:25
@Desc: 
***************************/

#ifndef CGRAPH_GSTAGE_H
#define CGRAPH_GSTAGE_H

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "GStageObject.h"

CGRAPH_NAMESPACE_BEGIN

class GStage : public GStageObject {
private:
    GStage() = default;

    /**
     * 设置阈值信息
     * @param threshold
     * @return
     */
    GStage* setThreshold(CInt threshold) {
        threshold_ = threshold;
        return this;
    }

    /**
     * 进入等待区域
     * @return
     */
    CVoid waiting() {
        {
            CGRAPH_LOCK_GUARD wm(waiting_mutex_);
            cur_value_++;
            if (cur_value_ >= threshold_) {
                // 如果超过了 threshold，则打开全部
                cur_value_ = 0;
                locker_.cv_.notify_all();
                return;
            }
        }

        CGRAPH_UNIQUE_LOCK lk(locker_.mtx_);
        locker_.cv_.wait(lk, [this] {
            return 0 == cur_value_ || cur_value_ >= threshold_;
        });
    }

private:
    CInt threshold_ { 0 };         // 阈值信息
    CInt cur_value_ { 0 };         // 当前值
    UCvMutex locker_;
    std::mutex waiting_mutex_;

    friend class GStageManager;
    friend class CAllocator;
};

using GStagePtr = GStage *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTAGE_H
