/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: E02-MockGUI.cpp
@Time: 2023/9/16 21:22
@Desc: 本example主要展示，在不修改之前任何功能（node中的逻辑）的情况下，
       通过aspect和event功能，在界面上展示当前正在执行的node，
       模拟 GUI 中node的变亮（或者变暗）的操作。
***************************/

#include <iostream>
#include <cmath>
#include <set>

#include "CGraph.h"

using namespace CGraph;

const static char* EXAMPLE_PARAM_KEY = "example-param-key";
const static char* EXAMPLE_EVENT_KEY = "example-event-key";

struct ProcessGParam : public GParam {
    CVoid change(const std::string& name, bool isBegin) {
        if (isBegin) {
            running_elements_.insert(name);
        } else {
            running_elements_.erase(name);
        }
    }

    CVoid print() {
        std::cout << "<";
        for (const auto& cur : running_elements_) {
            std::cout << " " << cur << " ";
        }
        std::cout << "> is running..." << std::endl;
    }

protected:
    std::set<std::string> running_elements_;
};


class ShowGEvent : public GEvent {
    CVoid trigger(GEventParamPtr param) override {
        auto p = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(ProcessGParam, EXAMPLE_PARAM_KEY);
        CGRAPH_PARAM_READ_CODE_BLOCK(p);
        p->print();
    }
};


class SwitchGAspect : public GAspect {
public:
    CStatus beginRun() override {
        auto p = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(ProcessGParam, EXAMPLE_PARAM_KEY);
        {
            CGRAPH_PARAM_WRITE_CODE_BLOCK(p)
            p->change(this->getName(), true);
        }

        notify(EXAMPLE_EVENT_KEY, GEventType::SYNC);
        return CStatus();
    }

    CVoid finishRun(const CStatus& curStatus) override {
        auto p = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(ProcessGParam, EXAMPLE_PARAM_KEY);
        {
            CGRAPH_PARAM_WRITE_CODE_BLOCK(p)
            p->change(this->getName(), false);
        }

        notify(EXAMPLE_EVENT_KEY, GEventType::SYNC);
    }
};


class ActionGNode : public GNode {
    CStatus run() override {
        int ms = std::abs((int)std::random_device{}()) % 4000 + 1000;
        CGRAPH_SLEEP_MILLISECOND(ms);    // 一个算子，随机休息一段时间，时长 1000~5000 ms
        return CStatus();
    }
};


void example_mock_gui() {
    GElementPtr a, b, c, d, e, f, g = nullptr;
    auto pipeline = GPipelineFactory::create();

    pipeline->registerGElement<ActionGNode>(&a, {}, "nodeA");
    pipeline->registerGElement<ActionGNode>(&b, {a}, "nodeB");
    pipeline->registerGElement<ActionGNode>(&c, {a}, "nodeC");
    pipeline->registerGElement<ActionGNode>(&d, {b}, "nodeD");
    pipeline->registerGElement<ActionGNode>(&e, {b, c}, "nodeE");
    pipeline->registerGElement<ActionGNode>(&f, {d, e}, "nodeF");
    pipeline->registerGElement<ActionGNode>(&g, {e}, "nodeG");

    pipeline->createGParam<ProcessGParam>(EXAMPLE_PARAM_KEY);
    pipeline->addGEvent<ShowGEvent>(EXAMPLE_EVENT_KEY);
    pipeline->addGAspect<SwitchGAspect>();    // 在每个node，开始和结束的时候，去触发 EXAMPLE_EVENT_KEY 事件，显示当前正在running的node信息

    pipeline->process();
    GPipelineFactory::clear();
}


int main() {
    example_mock_gui();
    return 0;
}
