/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEvent.h
@Time: 2023/1/20 23:07
@Desc: 
***************************/

#ifndef CGRAPH_GEVENT_H
#define CGRAPH_GEVENT_H

#include <vector>
#include <future>

#include "GEventObject.h"
#include "GEventDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GEvent : public GEventObject {
protected:
    /**
     * 事件被触发的时候，执行的函数
     * @param param 是pipeline那一层，注册进来的参数，可以为空
     * @return
     */
    virtual CVoid trigger(GEventParamPtr param) = 0;

    explicit GEvent();

    ~GEvent() override;

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

private:
    CStatus init() final;

    CStatus destroy() final;

    /**
     * 处理信号事件
     * @param type
     * @param strategy
     * @return
     */
    CStatus process(GEventType type, GEventAsyncStrategy strategy);

    /**
     * 等待并清理所有异步的event逻辑
     * @param strategy
     * @return
     */
    CVoid asyncWait(GEventAsyncStrategy strategy);

private:
    CBool is_init_ { false };                                        // 是否初始化
    std::vector<std::future<CVoid>> async_run_finish_futures_ {};    // 异步执行的逻辑集合（pipeline run结束的时候）
    std::vector<std::future<CVoid>> async_destroy_futures_ {};       // 异步执行的逻辑集合（pipeline destroy 的时候）
    GEventParamPtr param_ { nullptr };                               // 事件参数信息

    friend class GEventManager;
};

using GEventPtr = GEvent *;

CGRAPH_NAMESPACE_END


#endif //CGRAPH_GEVENT_H
