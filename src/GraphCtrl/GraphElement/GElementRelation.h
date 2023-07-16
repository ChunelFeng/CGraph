/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementRelation.h
@Time: 2023/5/24 23:15
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENTRELATION_H
#define CGRAPH_GELEMENTRELATION_H

#include "GElementObject.h"

CGRAPH_NAMESPACE_BEGIN

class GElement;

struct GElementRelation : public CStruct {
public:
    std::set<GElement *> predecessors_;       // 前驱信息
    std::set<GElement *> successors_;         // 后继信息
    GElement* belong_ = nullptr;              // 从属信息。如果从属于group，则为该group的值；如果从属于pipeline，则为nullptr
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTRELATION_H
