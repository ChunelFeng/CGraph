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
    using DistValType = float;    // 支持任意类型信息，如 float，int，double等
    const int dim = 4;

    std::vector<DistValType> vec1;
    std::vector<DistValType> vec2;
    for (int i = 0; i < dim; i++) {
        // 生成两个向量信息
        vec1.push_back((DistValType)i);
        vec2.push_back((DistValType)(i * i));
    }

    DistValType result = 0.0;
    // 计算欧氏距离
    UDistanceCalculator<DistValType, UDistanceEuclidean<DistValType> > eucDist;
    eucDist.calculate(vec1, vec2, result);
    std::cout << "UDistanceEuclidean distance result is : " << result << std::endl;

    /**
     * 支持自定义距离计算
     * 自定义距离的计算逻辑是：所有 向量1的元素乘以2，加上 向量2的元素除以2 之和
     * 故 vec1->vec2 的距离和 vec2->vec1的距离，是不同的。
     * 下例中给出对应的展示
     */
    UDistanceCalculator<DistValType, MyDistance<DistValType> > myDist;
    myDist.calculate(vec1, vec2, result);
    std::cout << "MyDistance distance vec1 -> vec2 result is : " << result << std::endl;

    myDist.calculate(vec2, vec1, result);
    std::cout << "MyDistance distance vec2 -> vec1 result is : " << result << std::endl;
}

int main() {
    tutorial_distance();
    return 0;
}

