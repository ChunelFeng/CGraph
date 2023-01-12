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
    clear();
    CGRAPH_FUNCTION_END
}


CStatus GParamManager::destroy() {
    CGRAPH_FUNCTION_BEGIN
    clear();
    CGRAPH_FUNCTION_END
}


CStatus GParamManager::clear() {
    CGRAPH_FUNCTION_BEGIN
    for (auto& param : params_map_) {
        CGRAPH_DELETE_PTR(param.second)
    }

    params_map_.clear();
    CGRAPH_FUNCTION_END;
}


CStatus GParamManager::reset() {
    CGRAPH_FUNCTION_BEGIN
    for (auto cur : params_map_) {
        if (likely(cur.second)) {
            cur.second->reset();
        }
    }
    CGRAPH_FUNCTION_END
}


CStatus GParamManager::remove(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(this->lock_);    // 创建和销毁的时候，加锁
    auto param = params_map_.find(key);
    if (param == params_map_.end()) {
        CGRAPH_RETURN_ERROR_STATUS("param [" + key + "] no find")
    }

    CGRAPH_DELETE_PTR(param->second)
    params_map_.erase(param);
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
