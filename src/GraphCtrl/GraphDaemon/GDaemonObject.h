/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemonObject.h
@Time: 2022/2/2 9:36 下午
@Desc:
***************************/

#ifndef CGRAPH_GDAEMONOBJECT_H
#define CGRAPH_GDAEMONOBJECT_H

#include "../GraphObject.h"
#include "../GraphParam/GParamInclude.h"
#include "../GraphEvent/GEventInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GDaemonObject : public GraphObject,
                      public CDescInfo {
protected:
    explicit GDaemonObject() {
        session_ = URandom<>::generateSession(CGRAPH_STR_DAEMON);
    }

    ~GDaemonObject() override {
        CGRAPH_DELETE_PTR(param_)
    }

    /**
     * 设置休眠时间信息，单位ms
     * @return
     */
    virtual GDaemonObject* setInterval(CMSec interval) {
        if (0 == interval) {
            return this;
        }

        interval_ = interval;
        return this;
    }

    /**
     * 设置daemon中参数，类型为GDaemonParam (即：GPassedParam)
     * @tparam T
     * @param param
     * @return
     */
    template <typename DParam,
            c_enable_if_t<std::is_base_of<GDaemonParam, DParam>::value, int> = 0>
    GDaemonObject* setDParam(DParam* param);

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

    CGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER

private:
    /**
     * 所有Daemon均不执行run方法
     * @return
     */
    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }

    CGRAPH_NO_ALLOWED_COPY(GDaemonObject)

    friend class GDaemon;
    friend class GDaemonManager;
    friend class GPipeline;

private:
    GParamManagerPtr param_manager_ = nullptr;                 // GParam参数管理类
    GEventManagerPtr event_manager_ = nullptr;                 // 事件管理类
    GDaemonParamPtr param_ = nullptr;                          // 用于存储daemon对象
    CMSec interval_ = 0;                                       // 执行间隔时间
};

using GDaemonObjectPtr = GDaemonObject *;

CGRAPH_NAMESPACE_END

#include "GDaemonObject.inl"

#endif //CGRAPH_GDAEMONOBJECT_H
