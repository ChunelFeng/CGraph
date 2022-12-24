/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTemplateNode.h
@Time: 2022/12/24 12:21
@Desc: 
***************************/

#ifndef CGRAPH_GTEMPLATENODE_H
#define CGRAPH_GTEMPLATENODE_H

#include "GNode.h"

CGRAPH_NAMESPACE_BEGIN

template<typename ...Args>
class GTemplateNode : public GNode {
};

template<typename ...Args>
using GTemplateNodePtr = GTemplateNode<Args ...> *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTEMPLATENODE_H
