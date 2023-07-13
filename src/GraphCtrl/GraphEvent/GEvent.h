/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEvent.h
@Time: 2023/1/20 23:07
@Desc: 
***************************/

#ifndef CGRAPH_GEVENT_H
#define CGRAPH_GEVENT_H

#include <mutex>

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

    friend class GEventManager;
};

using GEventPtr = GEvent *;

CGRAPH_NAMESPACE_END


#endif //CGRAPH_GEVENT_H
