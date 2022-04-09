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

template<typename TSrc, typename TRes>
CStatus UDistance<TSrc, TRes>::check(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, CVoidPtr ext) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(v1)
    CGRAPH_ASSERT_NOT_NULL(v2)
    if (0 == dim1 * dim2) {
        // 理论上不应该传入 dim=0 向量
        CGRAPH_RETURN_ERROR_STATUS("input dim error")
    }

    CGRAPH_FUNCTION_END
}


template<typename TSrc, typename TRes>
CStatus UDistance<TSrc, TRes>::normalize(TSrc* v, CSize dim, CVoidPtr ext) {
    CGRAPH_FUNCTION_BEGIN

    /** 这里不需要判定v为空的情况，需要的话可以通过开启 needCheck 逻辑来判断 */
    TSrc val = 0;
    for (CSize i = 0; i < dim; i++) {
        val += (v[i] * v[i]);
    }

    const TSrc& denominator = 1 / std::sqrt(val);    // 分母信息
    for (CSize i = 0; i < dim; i++) {
        v[i] = v[i] * denominator;
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UDISTANCE_INL