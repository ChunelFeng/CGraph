/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UEuclideanDistance.h
@Time: 2023/1/25 23:17
@Desc: 
***************************/

#ifndef CGRAPH_UEUCLIDEANDISTANCE_H
#define CGRAPH_UEUCLIDEANDISTANCE_H

#include <cmath>

#include "../UDistance.h"

CGRAPH_NAMESPACE_BEGIN

template<typename TSrc, typename TRes = TSrc, const CBool needSqrt = true>    // 默认给出的值，是开平方后的值
class UEuclideanDistance : public UDistance<TSrc, TRes> {
public:
    CStatus calc(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, TRes& result, CVoidPtr ext) override {
        CGRAPH_FUNCTION_BEGIN
        result = 0;
        for (CSize i = 0; i < dim1; i++) {
            result += std::pow(v1[i] - v2[i], 2);
        }

        result = needSqrt ? std::sqrt(result) : result;
        CGRAPH_FUNCTION_END
    }


    CStatus check(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, CVoidPtr ext) override {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_NOT_NULL(v1)
        CGRAPH_ASSERT_NOT_NULL(v2)
        if (dim1 != dim2 || 0 == dim1 * dim2) {
            // 不相等，或者有任何一个值为0，则认为是异常
            CGRAPH_RETURN_ERROR_STATUS("euclidean distance dim error")
        }

        CGRAPH_FUNCTION_END
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UEUCLIDEANDISTANCE_H
