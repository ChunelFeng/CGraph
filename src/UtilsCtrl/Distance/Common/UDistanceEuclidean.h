/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UDistanceEuclidean.h
@Time: 2022/4/2 00:36
@Desc: 欧氏距离计算方式
***************************/

#ifndef CGRAPH_UDISTANCEEUCLIDEAN_H
#define CGRAPH_UDISTANCEEUCLIDEAN_H

#include <cmath>

#include "../UDistance.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T = CFloat, const CBool needSqrt = true>    // 默认给出的值，是开平方后的值
class UDistanceEuclidean : public UDistance<T> {
public:
    CStatus calc(const T* v1, const T* v2, CSize dim1, CSize dim2, T& result, CVoidPtr ext) override {
        CGRAPH_FUNCTION_BEGIN
        result = 0;
        for (CSize i = 0; i < dim1; i++) {
            result += std::pow(v1[i] - v2[i], 2);
        }

        result = needSqrt ? std::sqrt(result) : result;
        CGRAPH_FUNCTION_END
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UDISTANCEEUCLIDEAN_H
