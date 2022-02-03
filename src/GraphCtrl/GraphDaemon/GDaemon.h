/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemon.h
@Time: 2022/2/2 9:54 下午
@Desc: 
***************************/

#ifndef CGRAPH_GDAEMON_H
#define CGRAPH_GDAEMON_H

#include <set>

#include "GDaemonObject.h"
#include "../GraphParam/GParamInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GDaemon : public GDaemonObject {

protected:
    /**
     * 获取休眠时间信息，单位ms
     * @return
     */
    virtual CMSec getInterval() = 0;

    /**
     * 后台执行函数，间隔interval时间后，执行一次
     */
    virtual CVoid daemonRun() = 0;

    CStatus init() final;

    CStatus destroy() final;

    friend class GDaemonManager;
    friend class GPipeline;

private:
    GParamManagerPtr param_manager_ = nullptr;        // 获取参数管理类
    UTimer timer_;
};

using GDaemonPtr = GDaemon *;
using GDaemonSet = std::set<GDaemonPtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GDAEMON_H
