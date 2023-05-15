/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEventObject.h
@Time: 2023/1/20 23:09
@Desc: 
***************************/

#ifndef CGRAPH_GEVENTOBJECT_H
#define CGRAPH_GEVENTOBJECT_H

#include "../GraphObject.h"
#include "../GraphParam/GParamInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GEventObject : public GraphObject,
                     public CDescInfo {
protected:
    explicit GEventObject() {
        session_ = URandom<>::generateSession(CGRAPH_STR_EVENT);
    }

    ~GEventObject() override {
        CGRAPH_DELETE_PTR(param_)
    }

    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }

    virtual GEventObject* setThreadPool(UThreadPoolPtr ptr) {
        thread_pool_ = ptr;
        return this;
    }

protected:
    UThreadPoolPtr thread_pool_ = nullptr;                   // 线程池类
    GParamManagerPtr param_manager_ = nullptr;               // GParam参数管理类
    GEventParamPtr param_ = nullptr;                         // 事件参数信息
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GEVENTOBJECT_H
