/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: DAnnTemplateNode.h
@Time: 2023/6/11 17:42
@Desc: 
***************************/

#ifndef CGRAPH_DANNTEMPLATENODE_H
#define CGRAPH_DANNTEMPLATENODE_H

#include "DAnnNode.h"

CGRAPH_NAMESPACE_BEGIN

template<typename ...Args>
class DAnnTemplateNode : public DAnnNode,
                         public GTemplateNode<Args ...> {
protected:
    CStatus run() override {
        // 复用 DAnnNode的执行方式
        return DAnnNode::run();
    }

    friend class GPipeline;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_DANNTEMPLATENODE_H
