/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: E05-HttpServer.cpp
@Time: 2026/2/15 16:28
@Desc: 本例主要展示，在server端创建n个 pipeline，放入manager中
       在被client请求的时候，会从manager 中取出一个，并且执行，然后再放回
***************************/

#include "CGraph.h"

using namespace CGraph;

const static char* EXAMPLE_PARAM_KEY = "/http/server/param_key";

struct HttpParam : public GParam {
    CStatus setup() override {
        val_ = 0;
        return CStatus();
    }

    int val_ {0};
    int loop_ {0};
};


class ReqeustNode : public GNode {
    CStatus init() override {
        auto status = CGRAPH_CREATE_GPARAM(HttpParam, EXAMPLE_PARAM_KEY);
        return status;
    }

    CStatus run() final {
        CGRAPH_ECHO("    ----> enter request node");
        return CStatus();
    }
};


template<int STEP, int MS>
class ProcessNode : public GNode {
    CStatus run() override {
        auto param = CGRAPH_GET_GPARAM_WITH_NO_EMPTY(HttpParam, EXAMPLE_PARAM_KEY);
        {
            // 从这里可以看出来，manager 中的每个pipeline，实际上都是
            CGRAPH_PARAM_WRITE_CODE_BLOCK(param);
            CGRAPH_ECHO("process step %d, sleep %d ms, val is %d, loop is %d",
                STEP, MS, ++param->val_, ++param->loop_);
        }
        CGRAPH_SLEEP_MILLISECOND(MS);
        return CStatus();
    }
};


class ResponseNode : public GNode {
    CStatus run() override {
        CGRAPH_ECHO("    <---- enter response node");
        return CStatus();
    }
};


GPipeline* build_pipeline() {
    GPipeline* pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;
    pipeline->registerGElement<ReqeustNode>(&a, {}, "request");
    pipeline->registerGElement<ProcessNode<1, 100>>(&b, {a}, "process_1");
    pipeline->registerGElement<ProcessNode<2, 200>>(&c, {a}, "process_2");
    pipeline->registerGElement<ResponseNode>(&d, {b, c}, "response");

    return pipeline;
}


void mock_client_request(GPipelineManager& manager) {
    for (int i = 0; i < 10; i++) {
        /**
         * 不同的线程，直接调用manger的run，是互斥运行的
         * 相当于直接运行 manager中 的pipeline 中的run函数
         * 如果并发请求数量，超过 manager中 pipeline数量，则status返回报错。
         * 也可以通过 manager.fetch() 和 manager.release() 接口在外部控制pipeline的获取和释放
         */
        auto status = manager.run();
        if (status.isErr()) {
            CGRAPH_ECHO("manager run failed. err info is [%s]", status.getInfo().c_str());
        }
        CGRAPH_SLEEP_MILLISECOND(10);
    }
}


void example_http_server() {
    GPipelineManager manager;
    const int times = 3;
    for (int i = 0; i < times; i++) {
        auto pipeline = build_pipeline();
        manager.add(pipeline);    // 在manger中，加入3个pipeline信息
    }
    auto status = manager.init();    // 初始化 manager 中所有的pipeline信息

    std::vector<std::thread> thds;
    for (int i = 0; i < times; i++) {
        auto thd = std::thread([&] {
            mock_client_request(manager);
        });
        thds.emplace_back(std::move(thd));
    }

    for (int i = 0; i < times; i++) {
        thds[i].join();
    }

    manager.destroy();
    GPipelineFactory::clear();    // 这里析构所有的pipeline信息
}


int main() {
    example_http_server();
    return 0;
}