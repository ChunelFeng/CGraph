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

CGRAPH_NAMESPACE_BEGIN

class GDaemonObject : public GraphObject {
protected:
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
            std::enable_if_t<std::is_base_of<GDaemonParam, DParam>::value, int> = 0>
    GDaemonObject* setDParam(DParam* param);

private:
    /**
     * 所有Daemon均不执行run方法
     * @return
     */
    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }

    friend class GDaemon;
    friend class GDaemonManager;
    friend class GPipeline;

private:
    GDaemonParamPtr param_ = nullptr;                          // 用于存储daemon对象
    CMSec interval_ = 0;
};

using GDaemonObjectPtr = GDaemonObject *;

CGRAPH_NAMESPACE_END

#include "GDaemonObject.inl"

#endif //CGRAPH_GDAEMONOBJECT_H
