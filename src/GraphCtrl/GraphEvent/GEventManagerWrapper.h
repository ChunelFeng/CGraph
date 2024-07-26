/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEventManagerWrapper.h
@Time: 2023/5/17 22:02
@Desc: 
***************************/

#ifndef CGRAPH_GEVENTMANAGERWRAPPER_H
#define CGRAPH_GEVENTMANAGERWRAPPER_H

#include "GEvent.h"
#include "GEventManager.h"

CGRAPH_NAMESPACE_BEGIN

#define CGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER                                                     \
    /**
     * 设置统一管控信号类
     * @param em
     * @return
     */                                                                                           \
    virtual void* setGEventManager(GEventManagerPtr em) {                                         \
        this->event_manager_ = em;                                                                \
        return this;                                                                              \
    }                                                                                             \
                                                                                                  \
    /**
     * 触发一个事件
     * @param key
     * @param type
     * @return
     * @notice 返回值仅表示是否触发成功，不表示事件是否执行成功
     */                                                                                           \
    CStatus notify(const std::string& key, GEventType type,                                       \
                   GEventAsyncStrategy strategy = GEventAsyncStrategy::PIPELINE_RUN_FINISH) {     \
        CGRAPH_FUNCTION_BEGIN                                                                     \
        CGRAPH_ASSERT_NOT_NULL(this->event_manager_)                                              \
        status = this->event_manager_->trigger(key, type, strategy);                              \
        CGRAPH_FUNCTION_END                                                                       \
    }                                                                                             \
                                                                                                  \
    /**
     * 异步触发一个事件
     * @param key
     * @param type
     * @return
     */                                                                                                                 \
    std::shared_future<CVoid> asyncNotify(const std::string& key,                                                       \
                                          GEventAsyncStrategy strategy = GEventAsyncStrategy::PIPELINE_RUN_FINISH) {    \
        CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(this->event_manager_);                                                       \
        return this->event_manager_->asyncTrigger(key, strategy);                                                       \
   }                                                                                                                    \
                                                                                                                        \

#define CGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER_WITH_MEMBER                               \
private:                                                                                \
    GEventManagerPtr event_manager_ = nullptr;                                          \
protected:                                                                              \
    CGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER                                               \
                                                                                        \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GEVENTMANAGERWRAPPER_H
