/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyDistance.h
@Time: 2022/4/3 15:07
@Desc: 
***************************/

#ifndef CGRAPH_MYDISTANCE_H
#define CGRAPH_MYDISTANCE_H

#include "CGraph.h"

template<typename TSrc, typename TRes>    // 自定义距离类型
class MyDistance : public CGraph::UDistance<TSrc, TRes> {
public:
    CStatus calc(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, TRes& result, CVoidPtr ext) override {
        /**
         * 如果需要实现自定义距离
         * 只要生成 UDistance<TSrc, TRes> 的子类，并且实现其中的 calc 方法即可
         * normalize 和 check 函数可选择实现
         */
        CStatus status;
        result = 0;
        for (CSize i = 0; i < dim1; i++) {
            /* 自定义一种距离计算方式，距离的计算方式是：所有 向量1的元素乘以2，加上向量2的元素除以2 之和 */
            result += (v1[i] * 2 + v2[i] / 2);
        }

        return status;
    }
};

#endif //CGRAPH_MYDISTANCE_H
