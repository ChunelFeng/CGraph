/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UDistanceCalculator.h
@Time: 2022/4/1 23:48
@Desc: 用于距离计算的类，支持自定义扩展
***************************/

#ifndef CGRAPH_UDISTANCECALCULATOR_H
#define CGRAPH_UDISTANCECALCULATOR_H

#include <vector>

#include "UDistanceObject.h"
#include "UDistance.h"

CGRAPH_NAMESPACE_BEGIN

template<typename TSrc, typename TRes, typename UDist,
        const CBool needCheck = false,
        c_enable_if_t<std::is_base_of<UDistance<TSrc, TRes>, UDist>::value, int> = 0>
class UDistanceCalculator : public UDistanceObject {
public:
    /**
     * 计算距离信息
     * @param v1
     * @param v2
     * @param dim1
     * @param dim2
     * @param result
     * @param ext
     * @return
     */
    CStatus calculate(const TSrc* v1, const TSrc* v2,
                      CSize dim1, CSize dim2,
                      TRes& result,
                      CVoidPtr ext = nullptr) {
        CGRAPH_FUNCTION_BEGIN
        status = needCheck ? distance_.check(v1, v2, dim1, dim2, ext) : CStatus();
        CGRAPH_FUNCTION_CHECK_STATUS

        status = distance_.calc(v1, v2, dim1, dim2, result, ext);
        CGRAPH_FUNCTION_END
    }

    /**
     * 计算距离信息
     * @param v1
     * @param v2
     * @param result
     * @param ext
     * @return
     */
    CStatus calculate(const std::vector<TSrc>& v1,
                      const std::vector<TSrc>& v2,
                      TRes& result,
                      CVoidPtr ext = nullptr) {
        return calculate(v1.data(), v2.data(), v1.size(), v2.size(), result, ext);
    }

    /**
     * 批量计算距离信息
     * @param query
     * @param nodes
     * @param results
     * @param ext
     * @return
     */
    CStatus calculate(const std::vector<TSrc>& query,
                      const std::vector<std::vector<TSrc> >& nodes,
                      std::vector<TRes>& results,
                      CVoidPtr ext = nullptr) {
        CGRAPH_FUNCTION_BEGIN
        results.clear();
        results.reserve(nodes.size());    // 预分配空间信息
        for (const std::vector<TSrc>& node : nodes) {
            TRes result = 0;    // 在这里初始化一下，防止calculate()实现过程中，不复位result的情况
            status += calculate(query, node, result, ext);
            results.emplace_back(result);
        }
        CGRAPH_FUNCTION_END
    }

    /**
     * 将数据归一化
     * @param v
     * @param dim
     * @param ext
     * @return
     */
    CStatus normalize(TSrc* v, CSize dim, CVoidPtr ext = nullptr) {
        CGRAPH_FUNCTION_BEGIN
        // 这里强行将一个向量copy一份传入判断，目的是为了是的 normalize 和 calculate 的判断逻辑，保持一致
        status = needCheck ? distance_.check(v, v, dim, dim, ext) : CStatus();
        CGRAPH_FUNCTION_CHECK_STATUS

        status = distance_.normalize(v, dim, ext);
        CGRAPH_FUNCTION_END
    }

    /**
     * 数据归一化
     * @param v
     * @param ext
     * @return
     */
    CStatus normalize(std::vector<TSrc>& v, CVoidPtr ext = nullptr) {
        return normalize(v.data(), v.size(), ext);
    }


private:
    UDist distance_;        // 具体的距离计算类
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UDISTANCECALCULATOR_H
