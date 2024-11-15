/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemon.h
@Time: 2022/2/3 9:54 下午
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
     * 后台执行函数，间隔interval时间后，执行一次
     * @param param
     * @return
     */
    virtual CVoid daemonTask(GDaemonParamPtr param) = 0;

    /**
     * 修改下一次休眠的时间
     * @param param
     * @return
     * @notice 返回值 <=0 的时候，不生效。 > 0 的时候，仅针对下一次生效
     */
    virtual CMSec modify(GDaemonParamPtr param);

    /**
     * 获取设置的延时信息
     * @return
     */
    CMSec getInterval() const;

private:
    CStatus init() final;
    CStatus destroy() final;

    friend class GDaemonManager;
    friend class GPipeline;

private:
    UTimer timer_;                              // 计时器
};

using GDaemonPtr = GDaemon *;
using GDaemonSet = std::set<GDaemonPtr>;

CGRAPH_NAMESPACE_END

#include "GDaemonObject.inl"

#endif //CGRAPH_GDAEMON_H
