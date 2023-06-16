/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMultiParallelCondition.h
@Time: 2023/6/16 00:05
@Desc: 
***************************/

#ifndef CGRAPH_GMULTIPARALLELCONDITION_H
#define CGRAPH_GMULTIPARALLELCONDITION_H

#include "GCondition.h"

CGRAPH_NAMESPACE_BEGIN

class GMultiParallelCondition : public GCondition {
protected:
    explicit GMultiParallelCondition();

    CStatus run() override;

private:
    CIndex choose() final;

    friend class GPipeline;
    friend class UAllocator;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMULTIPARALLELCONDITION_H
