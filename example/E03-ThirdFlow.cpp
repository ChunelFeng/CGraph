/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: E03-ThirdFlow.cpp
@Time: 2023/9/22 20:35
@Desc: 本example主要展示，常见的三流模型，使用CGraph如何表示
***************************/

#include <cmath>
#include <memory>

#include "CGraph.h"

using namespace CGraph;

static const char* INPUT_TOPIC_NAME = "example/input/topic";
static const char* RESULT_TOPIC_NAME = "example/result/topic";

int randomSleep(int start, int range) {
    // 随机休息一下，模拟处理的耗时
    int ms = std::abs((int)std::random_device{}()) % range + start;
    CGRAPH_SLEEP_MILLISECOND(ms);
    return ms;
}

struct InputMParam : public GMessageParam {
    int num_ = 0;
};

struct ResultMParam : public GMessageParam {
    int negative_num_ = 0;
    int process_ts_ = 0;
    std::string eng_info_;
};

class InputGNode : public GNode {
public:
    CStatus run() override {
        for (int i = 0; i < 30; i++) {
            std::unique_ptr<InputMParam> input(new InputMParam());
            randomSleep(1, 5);    // 间隔1~6ms，发送一次
            input->num_ = std::abs((int)std::random_device{}()) % 5 + 1;
            CGRAPH_SEND_MPARAM(InputMParam, INPUT_TOPIC_NAME, input, GMessagePushStrategy::WAIT);
        }

        CGRAPH_ECHO("    ----> InputGNode run finished");
        return CStatus();
    }
};

class ProcessGNode : public GNode {
public:
    CStatus run() override {
        while (true) {
            std::unique_ptr<InputMParam> input = nullptr;
            auto status = CGRAPH_RECV_MPARAM_WITH_TIMEOUT(InputMParam, INPUT_TOPIC_NAME, input, 1000);
            if (status.isErr()) {
                break;    // 一阵子收不到消息了，就自动停止好了
            }

            int ms = randomSleep(1, 100);    // 模拟处理流程，随机休息不超过 100ms
            std::unique_ptr<ResultMParam> result(new ResultMParam);
            switch (input->num_) {
                case 1: result->eng_info_ = "one"; break;
                case 2: result->eng_info_ = "two"; break;
                case 3: result->eng_info_ = "three"; break;
                default: result->eng_info_ = "other";
            }
            result->negative_num_ = -1 * input->num_;
            result->process_ts_ = ms;

            CGRAPH_SEND_MPARAM(ResultMParam, RESULT_TOPIC_NAME, result, GMessagePushStrategy::WAIT);
        }

        CGRAPH_ECHO("    ----> ProcessGNode run finished");
        return CStatus();
    }
};


class ResultGNode : public GNode {
public:
    CStatus run() override {
        while (true) {
            std::unique_ptr<ResultMParam> result = nullptr;
            auto status = CGRAPH_RECV_MPARAM_WITH_TIMEOUT(ResultMParam, RESULT_TOPIC_NAME, result, 1000);
            if (status.isErr()) {
                break;
            }

            randomSleep(1, 5);
            CGRAPH_ECHO("negative num is [%d], english info is [%s], process time is [%d]ms",
                        result->negative_num_, result->eng_info_.c_str(), result->process_ts_);
        }

        CGRAPH_ECHO("    ----> ResultGNode run finished");
        return CStatus();
    }
};

void example_third_flow() {
    CGRAPH_CREATE_MESSAGE_TOPIC(InputMParam, INPUT_TOPIC_NAME, 16);
    CGRAPH_CREATE_MESSAGE_TOPIC(ResultMParam, RESULT_TOPIC_NAME, 16);

    auto pipeline = GPipelineFactory::create();
    GElementPtr input, process, result = nullptr;
    pipeline->registerGElement<InputGNode>(&input);
    pipeline->registerGElement<ProcessGNode>(&process);
    pipeline->registerGElement<ResultGNode>(&result);

    UThreadPoolConfig config;
    config.default_thread_size_ = 3;
    config.secondary_thread_size_ = 0;
    pipeline->setUniqueThreadPoolConfig(config);    // 设置3个线程执行

    pipeline->process();
    CGRAPH_CLEAR_MESSAGES()
    GPipelineFactory::clear();
}

int main() {
    example_third_flow();
    return 0;
}
