/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/4/26 9:16 下午
@Desc: 构建图环境信息
***************************/

#include <thread>
#include <iostream>
#include <set>

using namespace std;

#include "Graphic.h"

Graphic::Graphic() {
    thread_pool_ = nullptr;
}

Graphic::~Graphic() {
    CGRAPH_DELETE_PTR(thread_pool_)
    std::for_each(nodes_.begin(), nodes_.end(), [](GraphNode* node){CGRAPH_DELETE_PTR(node)});
}

CSTATUS Graphic::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GraphNode* node : nodes_) {
        status = node->init();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_DELETE_PTR(thread_pool_)    // 确保pool_为空
    thread_pool_ = new GraphThreadPool();

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::deinit() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_DELETE_PTR(thread_pool_)

    for (GraphNode* node : nodes_){
        status = node->deinit();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::addGraphNode(GraphNode* node) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(node)

    this->nodes_.push_back(node);

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::run() {
    CGRAPH_FUNCTION_BEGIN

    int workedSize = 0;
    for (GraphNode* node : this->nodes_) {
        if (!node->isRunnable()) {
            continue;    // 如果暂时无法执行，则继续
        }

        queue_.push(node);
    }

    std::vector<GraphNode *> runnableNodes;
    std::vector<std::future<CSTATUS>> futures;

    runnableNodes.reserve(this->nodes_.size());
    futures.reserve(this->nodes_.size());

    while (!queue_.empty()) {
        /**
         * 1，将que_中可以执行的全部node，设定为 nodes
         * 2，并行计算 nodes 中的run方法
         * 3，等待全部执行结束，判定是否所有的返回值均ok
         * 4，如果均为ok，则继续计算 nodes 中的node后的节点信息
         **/
        runnableNodes.clear();
        futures.clear();

        while(!queue_.empty()) {
            runnableNodes.emplace_back(queue_.front());
            queue_.pop();
            workedSize++;
        }

        for (GraphNode* node : runnableNodes) {
            // 放到线程池中执行
            CGRAPH_ASSERT_NOT_NULL(node)
            futures.emplace_back(std::move(thread_pool_->commit(node)));
        }

        /* 这里相当于做了一层屏障，所有运行的线程停止后，再往下执行 */
        for (auto& fut : futures) {
            status = fut.get();
            CGRAPH_FUNCTION_CHECK_STATUS
        }

        std::set<GraphNode *> duplications;    // 防止当前流程中，单个节点重复添加，做去重使用
        for (GraphNode* node : runnableNodes) {
            for (GraphNode* cur : node->run_before_) {
                /**
                 * 1，节点未被执行过
                 * 2，节点处于可执行状态
                 * 3，节点之前未添加过
                 */
                if (cur->isRunnable()
                    && duplications.end() == duplications.find(cur)) {
                    queue_.push(cur);
                }
                duplications.insert(cur);
            }
        }
    }

    if (workedSize != nodes_.size()) {
        status = STATUS_ERR;    // 判定是否是所有的节点均被执行1次
    }

    CGRAPH_FUNCTION_END
}
