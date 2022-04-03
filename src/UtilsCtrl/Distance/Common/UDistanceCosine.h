/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UDistanceCosine.h
@Time: 2022/4/3 00:33
@Desc: cosine距离
***************************/

#ifndef CGRAPH_UDISTANCECOSINE_H
#define CGRAPH_UDISTANCECOSINE_H

#include "../UDistanceObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T = CFloat>
class UDistanceCosine : public UDistance<T> {
public:
    CStatus calc(const T* v1, const T* v2, CSize dim1, CSize dim2, T& result, CVoidPtr ext) override {
        CGRAPH_FUNCTION_BEGIN

        result = 0;     // 先清空
        T val1 = 0;    // 用于记录vec1的长度
        T val2 = 0;
        for (CSize i = 0; i < dim1; i++) {
            result += (v1[i] * v2[i]);
            val1 += (v1[i] * v1[i]);
            val2 += (v2[i] * v2[i]);
        }

        result = result / (std::sqrt(val1) * std::sqrt(val2));
        CGRAPH_FUNCTION_END
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UDISTANCECOSINE_H
