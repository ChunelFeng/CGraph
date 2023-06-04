/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.h
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#ifndef CGRAPH_GNODE_H
#define CGRAPH_GNODE_H

#include "GNodeDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GNode : public GElement {
protected:
    /**
     * 构造函数
     */
    explicit GNode();

    /**
     * 设置节点类型
     * @param type
     * @return
     */
    GNode* setType(const GNodeType& type);

    /**
     * 并发执行所有的task信息
     * @param tasks
     * @param ttl
     * @return
     */
    CStatus spawn(const UTaskGroup& tasks, CMSec ttl = CGRAPH_MAX_BLOCK_TTL);

private:
    GNodeType node_type_;                    // 节点类型
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODE_H