/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GFence.h
@Time: 2023/9/6 21:54
@Desc: 
***************************/

#ifndef CGRAPH_GFENCE_H
#define CGRAPH_GFENCE_H

#include <set>

#include "../GAdapter.h"

CGRAPH_NAMESPACE_BEGIN

class GFence : public GAdapter {
public:
    /**
     * 添加需要等待的element（必须是异步的）
     * @param element
     * @return
     * @notice 对外仅可以
     */
    GFence* waitGElement(GElementPtr element);

    /**
     * 添加需要等待的一组element
     * @param elements
     * @return
     */
    GFence* waitGElements(const std::set<GElementPtr>& elements);

    /**
     * 清空所有的elements信息
     * @return
     */
    CVoid clear();

protected:
    explicit GFence();

    CStatus checkSuitable() final;

    CVoid dumpElement(std::ostream& oss) final;

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

    CGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER

    CStatus run() final;

private:
    std::set<GElementPtr> fence_elements_;                           // 用于存放需要等待的值

    friend class GPipeline;
};

using GFencePtr = GFence *;
using GFencePPtr = GFencePtr *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GFENCE_H
