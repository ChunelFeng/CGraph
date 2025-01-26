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
protected:
    /**
     * stage 解开的时候进入的函数
     * @param param
     * @return
     */
    virtual CVoid launch(GStageParamPtr param);

protected:
    explicit GStage();
    ~GStage() override;

private:
    /**
     * 设置阈值信息
     * @param threshold
     * @return
     */
    GStage* setThreshold(CInt threshold);

    /**
     * 设置参数信息
     * @tparam T
     * @param param
     * @return
     */
    template <typename T,
            c_enable_if_t<std::is_base_of<GStageParam, T>::value, int> = 0>
    GStage* setSParam(T* param);

    /**
     * 进入等待区域
     * @return
     */
    CVoid waiting();

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER_WITH_MEMBER

    CGRAPH_NO_ALLOWED_COPY(GStage)

private:
    CInt threshold_ { 0 };                // 阈值信息
    CInt cur_value_ { 0 };                // 当前值
    GStageParamPtr param_ { nullptr };    // 参数信息
    UCvMutex locker_;
    std::mutex waiting_mutex_;

    friend class GStageManager;
    friend class CAllocator;
};

using GStagePtr = GStage *;

CGRAPH_NAMESPACE_END

#include "GStage.inl"

#endif //CGRAPH_GSTAGE_H
