/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCpuNode.h
@Time: 2022/1/24 9:55 下午
@Desc: 
***************************/

#ifndef CGRAPH_GCPUNODE_H
#define CGRAPH_GCPUNODE_H

CGRAPH_NAMESPACE_BEGIN

class GCpuNode : public GNode {
protected:
    explicit GCpuNode() {
        node_type_ = GNodeType::CPU;
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCPUNODE_H
