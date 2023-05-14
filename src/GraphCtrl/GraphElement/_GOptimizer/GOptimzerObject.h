/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GOptimzerObject.h
@Time: 2023/5/14 14:28
@Desc: 
***************************/

#ifndef CGRAPH_GOPTIMZEROBJECT_H
#define CGRAPH_GOPTIMZEROBJECT_H

#include "../GElementObject.h"

CGRAPH_NAMESPACE_BEGIN

class GOptimizerObject : public GElementObject {
protected:
    /**
     * 判定是否满足优化的条件
     * @param elements
     * @return
     */
    virtual CBool match(const GSortedGElementPtrSet& elements) = 0;

    CStatus run() override {
        CGRAPH_EMPTY_FUNCTION
    }
};

CGRAPH_NAMESPACE_END


#endif //CGRAPH_GOPTIMZEROBJECT_H
