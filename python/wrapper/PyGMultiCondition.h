/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyGMultiCondition.h
@Time: 2025/1/30 21:40
@Desc: 
***************************/

#ifndef CGRAPH_PYGMULTICONDITION_H
#define CGRAPH_PYGMULTICONDITION_H

#include "CGraph.h"

template<CGraph::GMultiConditionType type>
class PyGMultiCondition : public CGraph::GMultiCondition<type> {
public:
    explicit PyGMultiCondition() : CGraph::GMultiCondition<type>() {};
    ~PyGMultiCondition() override {};

    CStatus addGElement(CGraph::GElementPtr element) {
        return CGraph::GMultiCondition<type>::addElement(element);
    }
};

#endif //CGRAPH_PYGMULTICONDITION_H
