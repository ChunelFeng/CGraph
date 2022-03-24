/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.h
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#ifndef CGRAPH_GNODE_H
#define CGRAPH_GNODE_H

#include "../GElement.h"
#include "GNodeDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GNode : public GElement {
protected:
    explicit GNode() {
        node_type_ = GNodeType::BASIC;
    }

    /**
     * 异步执行信息，适用于传入静态类函数或者lambda表达式信息
     * @tparam Func
     * @tparam Args
     * @param func
     * @param args
     */
    template<typename Func, typename... Args>
    static CStatus doDetach(const Func&& func, Args&&... args) {
        CGRAPH_FUNCTION_BEGIN
        UThreadPoolPtr tp = UThreadPoolSingleton::get(false);
        CGRAPH_ASSERT_NOT_NULL(tp)

        tp->commit(std::bind(func, std::forward<Args>(args)...));
        CGRAPH_FUNCTION_END
    }

    /**
     * 执行批量任务
     * @param tasks
     * @param ttl
     * @return
     */
    CStatus doParallel(const UTaskGroup& tasks, CMSec ttl) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        UThreadPoolPtr tp = UThreadPoolSingleton::get(false);
        CGRAPH_ASSERT_NOT_NULL(tp)

        status = tp->submit(tasks, ttl);
        CGRAPH_FUNCTION_END
    }

    /**
    * 获取当前执行的线程id信息
    * @return
    */
    static CSize getThreadId() {
        const CSize& tid = (CSize) std::hash<std::thread::id>{}(std::this_thread::get_id());
        return tid;
    }


protected:
    GNodeType node_type_;                    // 节点类型
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODE_H
