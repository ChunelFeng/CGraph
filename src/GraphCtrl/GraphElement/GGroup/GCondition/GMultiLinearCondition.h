/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMultiLinearCondition.h
@Time: 2023/6/16 22:09
@Desc: 
***************************/

#ifndef CGRAPH_GMULTILINEARCONDITION_H
#define CGRAPH_GMULTILINEARCONDITION_H

#include "GCondition.h"

CGRAPH_NAMESPACE_BEGIN

class GMultiLinearCondition : public GCondition {
protected:
    explicit GMultiLinearCondition();

    CStatus run() override;

private:
    CIndex choose() final;

    friend class GPipeline;
    friend class UAllocator;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMULTILINEARCONDITION_H
