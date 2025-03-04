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
    explicit PyGMultiCondition(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        CGraph::GMultiCondition<type>::__addElements_4py(elements);
    };
    ~PyGMultiCondition() override {};
};

#endif //CGRAPH_PYGMULTICONDITION_H
