/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyDistance.h
@Time: 2022/4/3 15:07
@Desc: 
***************************/

#ifndef CGRAPH_MYDISTANCE_H
#define CGRAPH_MYDISTANCE_H

#include "../../src/CGraph.h"

template<typename T>    // 自定义距离类型
class MyDistance : public CGraph::UDistance<T> {
public:
    CStatus calc(const T* v1, const T* v2, CSize dim1, CSize dim2, T& result, CVoidPtr ext) override {
        /**
         * 如果需要实现自定义距离
         * 只要生成 UDistance<T> 的子类，并且实现其中的 calc 方法即可
         * normalize 和 check 函数可选择实现
         */
        CStatus status;
        result = 0;
        for (CSize i = 0; i < dim1; i++) {
            result += (v1[i] + v2[i]);    // 自定义一种距离计算方式，距离的计算方式，就是两个向量所有元素之和
        }

        return status;
    }
};

#endif //CGRAPH_MYDISTANCE_H
