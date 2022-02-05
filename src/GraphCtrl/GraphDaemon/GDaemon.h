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
     */
    virtual CVoid daemonTask() = 0;

    /**
     * 获取设置的延时信息
     * @return
     */
    [[nodiscard]] CMSec getInterval() const;

    /**
     * 获取参数信息
     * @tparam T
     * @param key
     * @return
     */
    template <typename T,
            std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    T* getGParam(const std::string& key);

private:
    CStatus init() final;
    CStatus destroy() final;

    friend class GDaemonManager;
    friend class GPipeline;

private:
    UTimer timer_;                // 计时器
};

using GDaemonPtr = GDaemon *;
using GDaemonSet = std::set<GDaemonPtr>;

CGRAPH_NAMESPACE_END

#include "GDaemon.inl"

#endif //CGRAPH_GDAEMON_H
