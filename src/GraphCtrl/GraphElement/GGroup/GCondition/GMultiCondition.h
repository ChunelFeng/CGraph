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
protected:
    explicit GMultiCondition();

private:
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

    CBool isSerializable() const override;

    CBool isSeparate(GElementCPtr a, GElementCPtr b) const final;

    friend class GPipeline;
    friend class CAllocator;
};

CGRAPH_NAMESPACE_END

#include "GMultiCondition.inl"

#endif //CGRAPH_GMULTICONDITION_H
