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

template<typename T, typename UD, const CBool needCheck = false,
        std::enable_if_t<std::is_base_of<UDistance<T>, UD>::value, int> = 0>
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
    CStatus calculate(const T* v1, const T* v2,
                      CSize dim1, CSize dim2,
                      T& result,
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
    CStatus calculate(const std::vector<T>& v1,
                      const std::vector<T>& v2,
                      T& result,
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
    CStatus calculate(const std::vector<T>& query,
                      const std::vector<std::vector<T> >& nodes,
                      std::vector<T>& results,
                      CVoidPtr ext = nullptr) {
        CGRAPH_FUNCTION_BEGIN
        results.clear();
        results.reserve(nodes.size());    // 预分配空间信息
        for (const std::vector<T>& node : nodes) {
            T result;
            status += calculate(query, node, result, ext);
            results.emplace_back(result);
        }
        CGRAPH_FUNCTION_END
    }

    /**
     * 将数据归一化
     * @param v
     * @param dim
     * @return
     */
    CStatus normalize(T* v, CSize dim) {
        return distance_.normalize(v, dim);
    }

    /**
     * 数据归一化
     * @param v
     * @return
     */
    CStatus normalize(std::vector<T>& v) {
        return normalize(v.data(), v.size());
    }


private:
    UD distance_;        // 具体的距离计算类
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UDISTANCECALCULATOR_H
