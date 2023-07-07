/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMultiCondition.h
@Time: 2023/6/17 19:21
@Desc: 
***************************/

#ifndef CGRAPH_GMULTICONDITION_H
#define CGRAPH_GMULTICONDITION_H

#include "GCondition.h"
#include "GConditionDefine.h"

CGRAPH_NAMESPACE_BEGIN

template<GMultiConditionType type>
class GMultiCondition : public GCondition {
private:
    explicit GMultiCondition();

    CStatus run() final;

    /**
     * 串行执行
     * @return
     */
    CStatus serialRun();

    /**
     * 并行执行
     * @return
     */
    CStatus parallelRun();

    CIndex choose() final;

    CBool isSerializable() override;

    friend class GPipeline;
    friend class UAllocator;
};

CGRAPH_NAMESPACE_END

#include "GMultiCondition.inl"

#endif //CGRAPH_GMULTICONDITION_H
