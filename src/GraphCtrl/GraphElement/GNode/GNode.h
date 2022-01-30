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
        node_type_ = GNodeType::DEFAULT;
    }


    /**
     * 异步执行信息，适用于传入静态类函数或者lambda表达式信息
     * @tparam Func
     * @tparam Args
     * @param func
     * @param args
     */
    template<typename Func, typename... Args>
    CStatus detach(const Func&& func, Args&&... args) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_INIT(true)

        UThreadPoolPtr pool = UThreadPoolSingleton::get(false);
        CGRAPH_ASSERT_NOT_NULL(pool)

        pool->commit(std::bind(func, std::forward<Args>(args)...));
        CGRAPH_FUNCTION_END
    }


protected:
    GNodeType node_type_;                    // 节点类型
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODE_H
