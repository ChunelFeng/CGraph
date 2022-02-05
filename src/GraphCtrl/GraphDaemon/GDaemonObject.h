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
    /**
     * 设置参数管理器，这里是统一设置。入参可以为空
     * @param pm
     * @return
     */
    virtual GDaemonObject* setPipelineParamManager(GParamManagerPtr pm) {
        CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(pm)
        this->pipeline_param_manager_ = pm;
        return this;
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
    GParamManagerPtr pipeline_param_manager_ = nullptr;        // 获取参数管理类
    CMSec interval_ = 0;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GDAEMONOBJECT_H
