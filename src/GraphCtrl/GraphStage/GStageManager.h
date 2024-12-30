/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStageManager.h
@Time: 2024/12/13 20:01
@Desc: 
***************************/

#ifndef CGRAPH_GSTAGEMANAGER_H
#define CGRAPH_GSTAGEMANAGER_H

#include <string>
#include <unordered_map>

#include "GStageObject.h"
#include "GStage.h"
#include "../GraphManager.h"

CGRAPH_NAMESPACE_BEGIN

class GStageManager : public GStageObject,
                      public GraphManager<GStage> {
protected:
    ~GStageManager() override {
        clear();
    }

    CStatus init() override {
        CGRAPH_FUNCTION_BEGIN
        for (auto stage : stage_map_) {
            status += stage.second->init();
        }
        CGRAPH_FUNCTION_END
    }

    CStatus destroy() override {
        CGRAPH_FUNCTION_BEGIN
        for (auto stage : stage_map_) {
            status += stage.second->destroy();
        }
        CGRAPH_FUNCTION_END
    }

    CStatus clear() final {
        CGRAPH_FUNCTION_BEGIN
        for (auto stage : stage_map_) {
            CGRAPH_DELETE_PTR(stage.second);
        }
        stage_map_.clear();
        CGRAPH_FUNCTION_END
    }

    /**
     * 创建 stage 信息
     * @tparam TStage
     * @tparam TParam
     * @param key
     * @param threshold
     * @param param
     * @return
     */
    template<typename TStage, typename TParam = GStageDefaultParam,
            c_enable_if_t<std::is_base_of<GStage, TStage>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GStageParam, TParam>::value, int> = 0>
    CStatus create(const std::string& key, CInt threshold, TParam* param) {
        CGRAPH_FUNCTION_BEGIN
        GStagePtr stage = CGRAPH_SAFE_MALLOC_COBJECT(TStage);
        stage->setThreshold(threshold);
        stage->setSParam(param);
        stage->setGParamManager(param_manager_);
        stage_map_.insert(std::pair<std::string, GStagePtr>(key, stage));
        CGRAPH_FUNCTION_END
    }

    /**
     * 进入等待区
     * @param key
     * @return
     */
    CStatus waitForReady(const std::string& key) {
        CGRAPH_FUNCTION_BEGIN
        auto iter = stage_map_.find(key);
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(stage_map_.end() == iter,
                                                "[" + key + "] is not a valid stage key")
        iter->second->waiting();
        CGRAPH_FUNCTION_END
    }

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER_WITH_MEMBER

private:
    std::unordered_map<std::string, GStagePtr> stage_map_ {};    // stage 集合

    friend class GPipeline;
    friend class GElement;
};

using GStageManagerPtr = GStageManager *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTAGEMANAGER_H
