/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UDistance.h
@Time: 2022/4/2 00:18
@Desc: 
***************************/

#ifndef CGRAPH_UDISTANCE_H
#define CGRAPH_UDISTANCE_H

#include "UDistanceObject.h"

CGRAPH_NAMESPACE_BEGIN

/** 传入的类型和计算结果的类型，可能不同。一般默认相同 */
template<typename TSrc, typename TRes = TSrc>
class UDistance : public UDistanceObject {
public:
    /**
     * 计算距离信息
     * @param v1 向量1
     * @param v2 向量2
     * @param dim1 向量1的维度
     * @param dim2 向量2的维度
     * @param result 结果信息
     * @param ext 可扩展信息
     * @return
     */
    virtual CStatus calc(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, TRes& result, CVoidPtr ext) = 0;

    /**
     * 判断入参信息是否符合
     * @param v1
     * @param v2
     * @param dim1
     * @param dim2
     * @param ext
     * @return
     */
    virtual CStatus check(const TSrc* v1, const TSrc* v2, CSize dim1, CSize dim2, CVoidPtr ext);

    /**
     * 将数据归一化
     * @param v
     * @param dim
     * @param ext
     * @return
     */
    virtual CStatus normalize(TSrc* v, CSize dim, CVoidPtr ext);
};

CGRAPH_NAMESPACE_END

#include "UDistance.inl"

#endif //CGRAPH_UDISTANCE_H
