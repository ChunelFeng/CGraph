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
     * 执行批量任务
     * @param tasks
     * @param ttl，单位是ms
     * @return
     */
    CStatus doParallel(const UTaskGroup& tasks, CMSec ttl = CGRAPH_MAX_BLOCK_TTL);

    /**
     * 设置节点类型
     * @param type
     * @return
     */
    GNode* setType(const GNodeType& type);

private:
    GNodeType node_type_;                    // 节点类型
};

using GNodePtr = GNode *;
using GNodePtrArr = std::vector<GNodePtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GNODE_H