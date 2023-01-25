/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEventManager.inl
@Time: 2023/1/17 22:56
@Desc:
***************************/

#ifndef CGRAPH_GEVENTMANAGER_INL
#define CGRAPH_GEVENTMANAGER_INL

#include "GEventManager.h"

CGRAPH_NAMESPACE_BEGIN

template<typename TEvent, typename TEParam,
        c_enable_if_t<std::is_base_of<GEvent, TEvent>::value, int>,
        c_enable_if_t<std::is_base_of<GEventParam, TEParam>::value, int>>
CStatus GEventManager::createWithParam(const std::string& key, TEParam* param) {
    CGRAPH_FUNCTION_BEGIN
    auto result = events_map_.find(key);
    if (events_map_.end() != result) {
        // 如果注册过了，则直接返回注册失败
        CGRAPH_RETURN_ERROR_STATUS("event [" + key + "] has been register")
    }

    GEventPtr event = CGRAPH_SAFE_MALLOC_COBJECT(TEvent)
    event->setGParamManager(this->param_manager_);

    // 每次创建，都是重新生成参数的一个过程
    CGRAPH_DELETE_PTR(event->param_)
    if (nullptr != param) {
        event->param_ = CGRAPH_SAFE_MALLOC_COBJECT(TEParam);
        event->param_->clone(param);
    }
    events_map_[key] = event;
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GEVENTMANAGER_INL
