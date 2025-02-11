/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStageManagerWrapper.h
@Time: 2024/12/15 17:48
@Desc: 
***************************/

#ifndef CGRAPH_GSTAGEMANAGERWRAPPER_H
#define CGRAPH_GSTAGEMANAGERWRAPPER_H

#include "GStage.h"
#include "GStageManager.h"

CGRAPH_NAMESPACE_BEGIN

#define CGRAPH_DECLARE_GSTAGE_MANAGER_WRAPPER_WITH_MEMBER             \
protected:                                                           \
    /**
     * 进入特定阶段，等待同步
     * @param key
     */                                                              \
    CStatus enterStage(const std::string& key) {                     \
        CGRAPH_FUNCTION_BEGIN                                        \
        CGRAPH_ASSERT_NOT_NULL(stage_manager_);                      \
        status = stage_manager_->waitForReady(key);                  \
        CGRAPH_FUNCTION_END                                          \
    }                                                                \
                                                                     \
private:                                                             \
    CVoidPtr setGStageManager(GStageManagerPtr sm) {                 \
        stage_manager_ = sm;                                         \
        return this;                                                 \
    }                                                                \
                                                                     \
private:                                                             \
    GStageManagerPtr stage_manager_ { nullptr };                     \
                                                                     \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTAGEMANAGERWRAPPER_H
