/***************************
@Author: LJX1021403
@Contact: 2898876973@qq.com
@File: E06-ParallelSort.cpp
@Time: 2026/9/2
@Desc: 本example展示如何利用CGraph并行执行多个排序任务。
       将一个大数组分成4块，分别交给4个SortGNode并行排序，
       最后通过四路归并合并结果，并与串行std::sort对比性能。
***************************/

#include <random>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

#include "CGraph.h"

using namespace CGraph;

std::mutex g_cout_mutex;

static const int PART_COUNT = 4;                // 4个并行排序节点
static const int NUMS_PER_PART = 250000;        // 每个节点处理25万个数字
static const int TOTAL_SIZE = PART_COUNT * NUMS_PER_PART;  // 总共100万个数字
static const char* PARAM_KEY = "sort-param-key";

// 用于在节点间共享数据的参数结构
struct SortGParam : public GParam {
    std::vector<int> data_;   // 待排序数据
};

// 生成随机数据的节点
class GenerateGNode : public GNode {
public:
    CStatus init() override {
        return CGRAPH_CREATE_GPARAM(SortGParam, PARAM_KEY);
    }

    CStatus run() override {
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(SortGParam, PARAM_KEY);
        std::mt19937 generator;
        std::uniform_int_distribution<int> distribution(0, 1000000);
        param->data_.resize(TOTAL_SIZE);
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
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(SortGParam, PARAM_KEY);
        auto& data = param->data_;

        // 四个有序区间的起始索引和结束索引
        std::vector<std::pair<int, int>> ranges = {
            {0, NUMS_PER_PART},
            {NUMS_PER_PART, 2 * NUMS_PER_PART},
            {2 * NUMS_PER_PART, 3 * NUMS_PER_PART},
            {3 * NUMS_PER_PART, TOTAL_SIZE}
        };

        // 四路归并
        std::vector<int> merged;
        merged.reserve(TOTAL_SIZE);
        std::vector<int> indices(4);
        std::vector<int> ends(4);
        for (int i = 0; i < 4; ++i) {
            indices[i] = ranges[i].first;
            ends[i] = ranges[i].second;
        }

        while (merged.size() < TOTAL_SIZE) {
            int minVal = INT32_MAX;
            int minIdx = -1;
            for (int i = 0; i < 4; ++i) {
                if (indices[i] < ends[i] && data[indices[i]] < minVal) {
                    minVal = data[indices[i]];
                    minIdx = i;
                }
            }
            if (minIdx == -1) break;
            merged.push_back(minVal);
            indices[minIdx]++;
        }

        std::copy(merged.begin(), merged.end(), data.begin());

        bool sorted = std::is_sorted(data.begin(), data.end());
        std::cout << "MergeGNode finished. Data sorted: " << (sorted ? "YES" : "NO") << std::endl;
        return CStatus();
    }
};

void example_parallel_sort() {
    // 用于串行排序基准的数据
    std::vector<int> initial_data(TOTAL_SIZE);
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(0, 1000000);
    for (auto& val : initial_data) {
        val = distribution(generator);
    }
    auto serial_data = initial_data;  // 拷贝一份用于串行排序

    // 1. 串行排序基准
    auto start_serial = std::chrono::steady_clock::now();
    std::sort(serial_data.begin(), serial_data.end());
    auto end_serial = std::chrono::steady_clock::now();
    double serial_ms = std::chrono::duration<double, std::milli>(end_serial - start_serial).count();

    // 2. 并行排序
    auto pipeline = GPipelineFactory::create();
    GElementPtr a, b0, b1, b2, b3, c = nullptr;

    CStatus status;
    status += pipeline->registerGElement<GenerateGNode>(&a, {});
    status += pipeline->registerGElement<SortGNode<0>>(&b0, {a});
    status += pipeline->registerGElement<SortGNode<1>>(&b1, {a});
    status += pipeline->registerGElement<SortGNode<2>>(&b2, {a});
    status += pipeline->registerGElement<SortGNode<3>>(&b3, {a});
    status += pipeline->registerGElement<MergeGNode>(&c, {b0, b1, b2, b3});

    if (status.isErr()) {
        CGRAPH_ECHO("register error: %s", status.getInfo().c_str());
        return;
    }

    auto start_parallel = std::chrono::steady_clock::now();
    status += pipeline->process();
    auto end_parallel = std::chrono::steady_clock::now();
    double parallel_ms = std::chrono::duration<double, std::milli>(end_parallel - start_parallel).count();

    if (status.isErr()) {
        CGRAPH_ECHO("process error: %s", status.getInfo().c_str());
        return;
    }

    // 输出性能对比
    std::cout << "Serial sort time: " << serial_ms << " ms" << std::endl;
    std::cout << "Parallel sort (CGraph) time: " << parallel_ms << " ms" << std::endl;
    std::cout << "Speedup: " << serial_ms / parallel_ms << "x" << std::endl;

    GPipelineFactory::clear();
}

int main() {
    example_parallel_sort();
    return 0;
}