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

class GEventObject : public GraphObject {
protected:
    explicit GEventObject() {
        thread_pool_ = UThreadPoolSingleton::get();
    }

    ~GEventObject() override {
        CGRAPH_DELETE_PTR(param_)
    }

    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }

protected:
    UThreadPoolPtr thread_pool_ = nullptr;                   // 线程池
    GParamManagerPtr param_manager_ = nullptr;               // GParam参数管理类
    GEventParamPtr param_ = nullptr;                         // 事件参数信息
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GEVENTOBJECT_H
