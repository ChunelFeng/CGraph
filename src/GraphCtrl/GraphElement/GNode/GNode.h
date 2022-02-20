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
    static CStatus detach(const Func&& func, Args&&... args) {
        CGRAPH_FUNCTION_BEGIN
        static UThreadPoolPtr s_pool = UThreadPoolSingleton::get(false);
        CGRAPH_ASSERT_NOT_NULL(s_pool)

        s_pool->commit(std::bind(func, std::forward<Args>(args)...));
        CGRAPH_FUNCTION_END
    }

    /**
    * 获取当前执行的线程id信息
    * @return
    */
    static size_t getThreadId() {
        const size_t& tid = std::hash<std::thread::id>{}(std::this_thread::get_id());
        return tid;
    }


protected:
    GNodeType node_type_;                    // 节点类型
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODE_H
