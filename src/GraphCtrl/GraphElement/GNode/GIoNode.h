/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GIoNode.h
@Time: 2022/1/24 9:56 下午
@Desc: 
***************************/

#ifndef CGRAPH_GIONODE_H
#define CGRAPH_GIONODE_H

CGRAPH_NAMESPACE_BEGIN

class GIoNode : public GNode {
protected:
    explicit GIoNode() {
        node_type_ = GNodeType::IO;
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GIONODE_H
