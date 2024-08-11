/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: E04-MapReduce.cpp
@Time: 2024/1/26 22:58
@Desc: 
***************************/

#include <random>
#include <vector>

#include "CGraph.h"

using namespace CGraph;

static const int MAP_SIZE = 4;
static const int NUMS_PER_MAP = 1000000;
static const int MAX_NUM = 10;
static const char* PARAM_KEY = "param-key";

struct NumsGParam : public GParam {
    std::vector<int> random_nums_ {};                   // 随机生成的数字信息
    int map_results_[MAP_SIZE][MAX_NUM] = {};           // 用于记录 map 处理结果的信息
};


class GenerateGNode : public GNode {
public:
    CStatus init() override {
        auto status = CGRAPH_CREATE_GPARAM(NumsGParam, PARAM_KEY);
        return status;
    }

    CStatus run() override {
        std::mt19937 generator;
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(NumsGParam, PARAM_KEY);

        std::uniform_int_distribution<int> distribution(0, MAX_NUM - 1);
        for (int i = 0; i < MAP_SIZE * NUMS_PER_MAP; i++) {
            int num = distribution(generator);
            param->random_nums_.push_back(num);
        }

        return CStatus();
    }
};


template<int MAP_TAG>
class MapGNode : public GNode {
public:
    CStatus run() override {
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(NumsGParam, PARAM_KEY);
        for (int i = 0; i < NUMS_PER_MAP; i++) {
            int num = param->random_nums_[MAP_TAG * NUMS_PER_MAP + i];
            param->map_results_[MAP_TAG][num]++;    // 实际上是线程安全的
        }
        return CStatus();
    }
};


class ReduceGNode : public GNode {
public:
    CStatus run() override {
        int total = 0;
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(NumsGParam, PARAM_KEY);
        for (int j = 0; j < MAX_NUM; j++) {
            int num = 0;
            for (int i = 0; i < MAP_SIZE; i++) {
                num += param->map_results_[i][j];
            }
            total += num;
            printf("  num [%d] appear [%d] times\n", j, num);
        }

        printf("total num is [%d]. \n", total);
        return CStatus();
    }
};


void example_map_reduce() {
    auto pipeline = GPipelineFactory::create();
    GElementPtr a, b0, b1, b2, b3, c = nullptr;

    CStatus status;
    status += pipeline->registerGElement<GenerateGNode>(&a, {});
    status += pipeline->registerGElement<MapGNode<0>>(&b0, {a});
    status += pipeline->registerGElement<MapGNode<1>>(&b1, {a});
    status += pipeline->registerGElement<MapGNode<2>>(&b2, {a});
    status += pipeline->registerGElement<MapGNode<3>>(&b3, {a});
    status += pipeline->registerGElement<ReduceGNode>(&c, {b0, b1, b2, b3});

    status += pipeline->process();
    if (status.isErr()) {
        CGRAPH_ECHO("example_map_reduce error info is : %s", status.getInfo().c_str());
    }

    GPipelineFactory::clear();
}


int main() {
    example_map_reduce();
    return 0;
}
