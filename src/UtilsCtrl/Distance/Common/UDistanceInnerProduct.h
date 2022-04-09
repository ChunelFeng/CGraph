/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UDistanceInnerProduct.h
@Time: 2022/4/2 22:36
@Desc: 内积距离
***************************/

#ifndef CGRAPH_UDISTANCEINNERPRODUCT_H
#define CGRAPH_UDISTANCEINNERPRODUCT_H

#include "../UDistance.h"

CGRAPH_NAMESPACE_BEGIN

template<typename TSrc, typename TRes = TSrc>
class UDistanceInnerProduct : public UDistance<TSrc, TRes> {
public:
    CStatus calc(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, TRes& result, CVoidPtr ext) override {
        CGRAPH_FUNCTION_BEGIN
        result = 0;
        for (CSize i = 0; i < dim1; i++) {
            result += (v1[i] * v2[i]);
        }

        // ip距离，默认是norm之后的数据，值小表示相似，范围是[0,1]，值=0.5表示正交
        result = (1 - result) * 0.5;
        CGRAPH_FUNCTION_END
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UDISTANCEINNERPRODUCT_H
