/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UDistance.inl
@Time: 2022/4/2 00:18
@Desc:
***************************/

#ifndef CGRAPH_UDISTANCE_INL
#define CGRAPH_UDISTANCE_INL

#include <cmath>

#include "UDistance.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
CStatus UDistance<T>::check(const T* v1, const T* v2, CSize dim1, CSize dim2, CVoidPtr ext) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(v1)
    CGRAPH_ASSERT_NOT_NULL(v2)
    if (0 == dim1 * dim2) {
        // 通常情况下，没有dim=0的向量
        status.setStatus("input dim error");
    }

    CGRAPH_FUNCTION_END
}


template<typename T>
CStatus UDistance<T>::normalize(T* v, CSize dim, CVoidPtr ext) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(v)

    T val = 0;
    for (CSize i = 0; i < dim; i++) {
        val += (v[i] * v[i]);
    }

    const T& denominator = 1 / std::sqrt(val);    // 分母信息
    for (CSize i = 0; i < dim; i++) {
        v[i] = v[i] * denominator;
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UDISTANCE_INL