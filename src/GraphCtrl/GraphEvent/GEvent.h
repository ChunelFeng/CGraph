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
#include <mutex>
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

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

private:
    /**
     * 处理信号事件
     * @param type
     * @return
     */
    CStatus process(GEventType type);

    /**
     * 执行结束后，清理所有异步的event逻辑
     * @return
     */
    CVoid wait();

private:
    std::vector<std::future<CVoid>> async_futures_;    // 异步执行的逻辑集合，用于确保每次pipeline执行的时候，异步的event不会被带入下一次逻辑

    friend class GEventManager;
};

using GEventPtr = GEvent *;

CGRAPH_NAMESPACE_END


#endif //CGRAPH_GEVENT_H
