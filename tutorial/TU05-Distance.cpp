/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: TU05-Distance.cpp
@Time: 2022/4/2 20:15
@Desc: 本例主要演示，UDistanceCalculator工具的使用方法
***************************/

#include <vector>

#include "MyUtils/MyDistance.h"

using namespace CGraph;

void tutorial_distance() {
    /**
     * 支持任意类型信息，如 float，int，double等
     * 并且支持结果类型转换
     */
    using DistSrcType = float;    // 向量值类型
    using DistResType = float;    // 计算结果类型
    const int dim = 16;

    std::vector<DistSrcType> vec1;
    std::vector<DistSrcType> vec2;

    // 随机生成两个 DistSrcType 类型，dim 维度，且值在[0.0~1.0]的向量
    URandom<DistSrcType>::generate(vec1, dim, 0.0, 1.0);
    URandom<DistSrcType>::generate(vec2, dim, 0.0, 1.0);

    DistResType result = 0.0;
    // 计算欧氏距离
    UDistanceCalculator<DistSrcType, DistResType, UEuclideanDistance<DistSrcType, DistResType> > eucDist;
    eucDist.calculate(vec1, vec2, result);
    std::cout << "UDistanceEuclidean distance result is : " << result << std::endl;

    /**
     * 支持自定义距离计算
     * 自定义距离的计算逻辑是：所有 向量1的元素乘以2，加上 向量2的元素除以2 之和
     * 故 vec1->vec2 的距离和 vec2->vec1的距离，是不同的。
     * 下例中给出对应的展示
     */
    UDistanceCalculator<DistSrcType, DistResType, MyDistance<DistSrcType, DistResType> > myDist;
    myDist.calculate(vec1, vec2, result);
    std::cout << "MyDistance distance vec1 -> vec2 result is : " << result << std::endl;

    myDist.calculate(vec2, vec1, result);
    std::cout << "MyDistance distance vec2 -> vec1 result is : " << result << std::endl;
}


int main() {
    tutorial_distance();
    return 0;
}

