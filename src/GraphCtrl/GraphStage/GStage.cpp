/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStage.cpp
@Time: 2024/12/28 23:02
@Desc: 
***************************/

#include "GStage.h"

CGRAPH_NAMESPACE_BEGIN

CVoid GStage::launch(GStageParamPtr param) {
}


GStage::GStage() {
    session_ = URandom<>::generateSession(CGRAPH_STR_STAGE);
}


GStage::~GStage() {
    CGRAPH_DELETE_PTR(param_);
}


GStagePtr GStage::setThreshold(CInt threshold) {
    threshold_ = threshold;
    return this;
}


CVoid GStage::waiting() {
    {
        CGRAPH_LOCK_GUARD wm(waiting_mutex_);
        cur_value_++;
        if (cur_value_ >= threshold_) {
            // 如果超过了 threshold，则打开全部
            launch(param_);
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

CGRAPH_NAMESPACE_END