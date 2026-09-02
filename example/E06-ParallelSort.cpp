/***************************
@Author: LJX1021403
@Contact: 2898876973@qq.com
@File: E06-ParallelSort.cpp
@Time: 2026/9/2
@Desc: 本example展示如何利用CGraph并行执行多个排序任务。
       将一个大数组分成4块，分别交给4个SortGNode并行排序
***************************/

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "CGraph.h"

using namespace CGraph;

static const int PART_COUNT = 4;                // 4个并行排序节点
static const int NUMS_PER_PART = 250000;        // 每个节点处理25万个数字
static const int TOTAL_SIZE = PART_COUNT * NUMS_PER_PART;  // 总共100万个数字
static auto PARAM_KEY = "sort-param-key";

// 用于在节点间共享数据的参数结构
struct SortGParam : public GParam {
    std::vector<int> data_;   // 待排序数据
    std::vector<int> result_;

protected:
    CStatus setup() override {
        result_.reserve(TOTAL_SIZE);
        data_.resize(TOTAL_SIZE);
        return CStatus();
    }
};


// 生成随机数据的节点
class GenerateGNode : public GNode {
public:
    CStatus init() override {
        return CGRAPH_CREATE_GPARAM(SortGParam, PARAM_KEY);
    }

    CStatus run() override {
        const auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(SortGParam, PARAM_KEY);
        std::mt19937 generator{};
        std::uniform_int_distribution<int> distribution(0, TOTAL_SIZE);
        for (auto& val : param->data_) {
            val = distribution(generator);
        }
        return CStatus();
    }
};


// 排序节点：负责对数据中的某一段进行排序
template<int PART_TAG>
class SortGNode : public GNode {
public:
    CStatus run() override {
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(SortGParam, PARAM_KEY);
        int begin = PART_TAG * NUMS_PER_PART;
        int end = begin + NUMS_PER_PART;
        std::sort(param->data_.begin() + begin, param->data_.begin() + end);
        return CStatus();
    }
};


// 合并节点：使用四路归并将四个已排序的片段合并成整体有序数组
class MergeGNode : public GNode {
public:
    CStatus run() override {
        const auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(SortGParam, PARAM_KEY);
        auto& data = param->data_;
        auto& result = param->result_;

        // 四个有序区间的起始索引和结束索引
        const std::vector<std::pair<int, int>> ranges = {
            {0, NUMS_PER_PART},
            {1 * NUMS_PER_PART, 2 * NUMS_PER_PART},
            {2 * NUMS_PER_PART, 3 * NUMS_PER_PART},
            {3 * NUMS_PER_PART, TOTAL_SIZE}
        };

        // 四路归并
        std::vector<int> indices(PART_COUNT);
        std::vector<int> ends(PART_COUNT);
        for (int i = 0; i < PART_COUNT; ++i) {
            indices[i] = ranges[i].first;
            ends[i] = ranges[i].second;
        }

        while (result.size() < TOTAL_SIZE) {
            int minVal = INT32_MAX;
            int minIdx = -1;
            for (int i = 0; i < PART_COUNT; ++i) {
                if (indices[i] < ends[i] && data[indices[i]] < minVal) {
                    minVal = data[indices[i]];
                    minIdx = i;
                }
            }
            if (minIdx != -1) {
                result.emplace_back(minVal);
                indices[minIdx]++;
            }
        }
        return CStatus();
    }
};


class CheckGNode : public GNode {
public:
    CStatus run() override {
        const auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(SortGParam, PARAM_KEY);
        const auto& result = param->result_;

        bool pass = std::is_sorted(result.begin(), result.end());
        std::cout << "parallel sort check result is: "<< (pass ? "PASS" : "FAIL") << std::endl;
        return CStatus();
    }
};


void example_parallel_sort() {
    auto pipeline = GPipelineFactory::create();
    GElementPtr a, b0, b1, b2, b3, c, d = nullptr;

    CStatus status;
    status += pipeline->registerGElement<GenerateGNode>(&a, {});
    status += pipeline->registerGElement<SortGNode<0>>(&b0, {a});
    status += pipeline->registerGElement<SortGNode<1>>(&b1, {a});
    status += pipeline->registerGElement<SortGNode<2>>(&b2, {a});
    status += pipeline->registerGElement<SortGNode<3>>(&b3, {a});
    status += pipeline->registerGElement<MergeGNode>(&c, {b0, b1, b2, b3});
    status += pipeline->registerGElement<CheckGNode>(&d, {c});

    pipeline->process();

    GPipelineFactory::clear();
}

int main() {
    example_parallel_sort();
    return 0;
}
