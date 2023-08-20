/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAsyncNode.h
@Time: 2023/8/10 21:37
@Desc: 
***************************/

#ifndef CGRAPH_GASYNCNODE_H
#define CGRAPH_GASYNCNODE_H

#include <future>

#include "GNode.h"

CGRAPH_NAMESPACE_BEGIN

class GAsyncNode : public GNode {
protected:
    /**
     * 异步执行的具体逻辑，确保会在当前pipeline执行结束完成之前，被执行完
     * @return
     */
    virtual CStatus asyncRun() = 0;

    /**
     * 异步获取结果信息
     * @return
     */
    CStatus getResult();

    explicit GAsyncNode();

private:
    CStatus run() final;

private:
    std::future<CStatus> async_result_;            // 用于记录当前节点的执行情况

    friend class GElementRepository;
    template<CSize> friend class GSome;
};

using GAsyncNodePtr = GAsyncNode *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASYNCNODE_H
