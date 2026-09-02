/***************************
@Author: LJX1021403
@Contact: 2898876973@qq.com
@File: E06-ParallelSort.cpp
@Time: 2026/9/2
@Desc: 本example展示如何利用CGraph并行执行多个排序任务。
       将一个大数组分成4块，分别交给4个SortGNode并行排序，
       最后合并结果并验证正确性。
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

static const int PART_COUNT = 4;            
static const int NUMS_PER_PART = 250000;    
static const int TOTAL_SIZE = PART_COUNT * NUMS_PER_PART;  
static const char* PARAM_KEY = "sort-param-key";

// 用于在节点间共享数据的参数结构
struct SortGParam : public GParam {
    std::vector<int> data_;   
};

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

        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "SortGNode [" << PART_TAG << "] start on thread "
                      << std::this_thread::get_id() << std::endl;
        }

        std::sort(param->data_.begin() + begin, param->data_.begin() + end);

        {
            std::lock_guard<std::mutex> lock(g_cout_mutex);
            std::cout << "SortGNode [" << PART_TAG << "] finished" << std::endl;
        }
        return CStatus();
    }
};

// 合并节点：将所有已排序的片段合并成一个有序数组（简单起见，整体再排序一次）
class MergeGNode : public GNode {
public:
    CStatus run() override {
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(SortGParam, PARAM_KEY);
        // 这里为了示例简洁，直接使用 std::sort 重新排序整个数组。
        std::sort(param->data_.begin(), param->data_.end());

        // 验证排序是否正确（检查是否升序）
        bool sorted = std::is_sorted(param->data_.begin(), param->data_.end());
        std::cout << "MergeGNode finished. Data sorted: " << (sorted ? "YES" : "NO") << std::endl;
        std::cout << "First 10 elements: ";
        for (int i = 0; i < 10 && i < (int)param->data_.size(); ++i) {
            std::cout << param->data_[i] << " ";
        }
        std::cout << std::endl;
        return CStatus();
    }
};

void example_parallel_sort() {
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

    // 记录并行执行时间
    auto start = std::chrono::steady_clock::now();
    status += pipeline->process();
    auto end = std::chrono::steady_clock::now();
    std::cout << "Parallel sort pipeline time: "
              << std::chrono::duration<double, std::milli>(end - start).count()
              << " ms" << std::endl;

    if (status.isErr()) {
        CGRAPH_ECHO("process error: %s", status.getInfo().c_str());
    }

    GPipelineFactory::clear();
}

int main() {
    example_parallel_sort();
    return 0;
}