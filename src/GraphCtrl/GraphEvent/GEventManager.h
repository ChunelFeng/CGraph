/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEventManager.h
@Time: 2023/1/20 23:08
@Desc: 
***************************/

#ifndef CGRAPH_GEVENTMANAGER_H
#define CGRAPH_GEVENTMANAGER_H

#include <string>
#include <unordered_map>
#include <future>

#include "GEvent.h"
#include "../GraphManager.h"

CGRAPH_NAMESPACE_BEGIN

class GEventManager : public GEventObject,
                      public GraphManager<GEvent> {
public:
    /**
     * 创建一个特定类型的事件，并且通过参数赋值
     * @tparam TEvent
     * @tparam TEParam
     * @param key
     * @param param
     * @return
     */
    template<typename TEvent, typename TEParam = GEventDefaultParam,
            c_enable_if_t<std::is_base_of<GEvent, TEvent>::value, int> = 0,
            c_enable_if_t<std::is_base_of<GEventParam, TEParam>::value, int> = 0>
    CStatus createWithParam(const std::string& key, TEParam* param = nullptr);

    /**
     * 触发事件
     * @param key
     * @param type
     * @param strategy, 仅当异步的时候生效
     * @return
     */
    CStatus trigger(const std::string &key, GEventType type, GEventAsyncStrategy strategy);

    /**
     * 异步事件触发
     * @param key
     * @param strategy
     * @return
     */
    std::shared_future<CVoid> asyncTrigger(const std::string &key, GEventAsyncStrategy strategy);

protected:
    CStatus init() final;

    CStatus destroy() final;

    CStatus clear() final;

    GEventObjectPtr setThreadPool(UThreadPoolPtr ptr) final;

    explicit GEventManager() = default;

    ~GEventManager() override;

    CStatus reset() final;

    CGRAPH_NO_ALLOWED_COPY(GEventManager)

private:
    std::unordered_map<std::string, GEventPtr> events_map_;    // event 管理类

    friend class UAllocator;
    friend class GPipeline;
};

using GEventManagerPtr = GEventManager *;

CGRAPH_NAMESPACE_END

#include "GEventManager.inl"

#endif //CGRAPH_GEVENTMANAGER_H
