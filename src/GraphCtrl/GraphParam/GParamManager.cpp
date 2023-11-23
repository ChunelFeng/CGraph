/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamManager.cpp
@Time: 2021/6/11 6:59 下午
@Desc: 
***************************/

#include "GParamManager.h"

CGRAPH_NAMESPACE_BEGIN

GParamManager::GParamManager() {
    clear();
}


GParamManager::~GParamManager() {
    clear();
}


CStatus GParamManager::init() {
    CGRAPH_FUNCTION_BEGIN
    for (auto& param : params_map_) {
        /**
         * 一般情况下，是不需要init的。需要注意init 和 setup的执行阶段的信息
         * init() 是在pipeline init的时候被执行的
         * setup() 是在pipeline run开始的时候被执行的
         */
        CGRAPH_ASSERT_NOT_NULL(param.second);
        status += param.second->init();
    }
    CGRAPH_FUNCTION_END
}


CStatus GParamManager::destroy() {
    CGRAPH_FUNCTION_BEGIN
    for (auto& param : params_map_) {
        CGRAPH_ASSERT_NOT_NULL(param.second);
        status += param.second->destroy();
    }
    CGRAPH_FUNCTION_CHECK_STATUS

    clear();
    CGRAPH_FUNCTION_END
}


CStatus GParamManager::clear() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(this->mutex_);
    for (auto& param : params_map_) {
        CGRAPH_DELETE_PTR(param.second)
    }

    params_map_.clear();
    CGRAPH_FUNCTION_END;
}


CVoid GParamManager::resetWithStatus(const CStatus& curStatus) {
    for (auto cur : params_map_) {
        cur.second->reset(curStatus);
    }
}


CStatus GParamManager::setup() {
    CGRAPH_FUNCTION_BEGIN
    for (auto cur : params_map_) {
        // 这里不需要判断非空，因为在init的时候，已经判断过了
        status += cur.second->setup();
    }
    CGRAPH_FUNCTION_END
}


CStatus GParamManager::removeByKey(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(this->mutex_);    // 创建和销毁的时候，加锁
    auto param = params_map_.find(key);
    if (param == params_map_.end()) {
        CGRAPH_RETURN_ERROR_STATUS("param [" + key + "] no find")
    }

    CGRAPH_DELETE_PTR(param->second)
    params_map_.erase(param);
    CGRAPH_FUNCTION_END
}


CStatus GParamManager::getKeys(std::vector<std::string>& keys) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(this->mutex_);
    for (const auto& iter : params_map_) {
        keys.emplace_back(iter.first);
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
