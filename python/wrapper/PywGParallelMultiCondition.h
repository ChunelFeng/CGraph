/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PywGParallelMultiCondition.h
@Time: 2025/4/8 00:09
@Desc: 
***************************/

#ifndef CGRAPH_PYWGPARALLELMULTICONDITION_H
#define CGRAPH_PYWGPARALLELMULTICONDITION_H

#include "CGraph.h"
#include "PyWrapperMacro.h"

class PywGParallelMultiCondition : public CGraph::GMultiCondition<CGraph::GMultiConditionType::PARALLEL> {
public:
    explicit PywGParallelMultiCondition(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        CGraph::GMultiCondition<CGraph::GMultiConditionType::PARALLEL>::__addGElements_4py(elements);
    };

    ~PywGParallelMultiCondition() override = default;
};

PYCGRAPH_DECLARE_GGROUP_INTERFACE_CLASS(PywGParallelMultiCondition, ;);

#endif //CGRAPH_PYWGPARALLELMULTICONDITION_H
