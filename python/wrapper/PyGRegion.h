/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: PyGRegion.h
@Time: 2025/1/30 21:44
@Desc: 
***************************/

#ifndef CGRAPH_PYGREGION_H
#define CGRAPH_PYGREGION_H

#include "CGraph.h"

class PyGRegion : public CGraph::GRegion {
public:
    explicit PyGRegion(const CGraph::GElementPtrArr& elements = CGraph::GElementPtrArr{}) {
        __addElements_4py(elements);
    };
    ~PyGRegion() override {};
};

#endif //CGRAPH_PYGREGION_H
