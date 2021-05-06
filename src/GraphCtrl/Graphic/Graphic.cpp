/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/4/26 9:16 下午
@Desc: 构建图环境信息
***************************/


#include <set>
#include "Graphic.h"

Graphic::Graphic() {
    thread_pool_ = nullptr;
    is_init_ = false;
}

Graphic::~Graphic() {
    CGRAPH_DELETE_PTR(thread_pool_)
    is_init_ = false;
    std::for_each(graph_nodes_.begin(), graph_nodes_.end(),
                  [](GraphNode* node){CGRAPH_DELETE_PTR(node)});
}

CSTATUS Graphic::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GraphNode* node : graph_nodes_) {
        status = node->init();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_DELETE_PTR(thread_pool_)    // 确保pool_为空
    thread_pool_ = new(std::nothrow) GraphThreadPool();
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)

    is_init_ = true;    // 初始化完毕，设置标记位

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::deinit() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_DELETE_PTR(thread_pool_)

    for (GraphNode* node : graph_nodes_) {
        status = node->deinit();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    is_init_ = false;

    CGRAPH_FUNCTION_END
}

CSTATUS Graphic::run() {
    CGRAPH_FUNCTION_BEGIN

    // 必须初始化之后，才可以执行run方法
    CGRAPH_ASSERT_INIT(true)

    int runNodeSize = 0;
    int totalNodeSize = graph_nodes_.size();
    for (GraphNode* node : this->graph_nodes_) {
        if (!node->isRunnable()) {
            continue;    // 如果暂时无法执行，则继续
        }

        queue_.push(node);
    }

    std::vector<GraphNode *> runnableNodes;
    std::vector<std::future<CSTATUS>> futures;

    runnableNodes.reserve(totalNodeSize);
    futures.reserve(totalNodeSize);

    while (!queue_.empty() || runNodeSize > totalNodeSize) {
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
            runNodeSize++;
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
                 * 1，节点未被执行过且处于可执行状态
                 * 2，节点之前未添加过
                 */
                if (cur->isRunnable()
                    && duplications.end() == duplications.find(cur)) {
                    queue_.push(cur);
                }
                duplications.insert(cur);
            }
        }
    }

    // 执行完毕后，确认是否所有节点均被执行过一次
    status = checkFinalStatus(runNodeSize);

    CGRAPH_FUNCTION_END
}

/**
 * 设置
 * @param node
 * @param dependNodes
 * @return
 */
CSTATUS Graphic::addDependNodes(GraphNode* node,
                                const std::set<GraphNode *>& dependNodes) const {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(node)

    for (GraphNode* cur : dependNodes) {
        // 如果传入的信息中，有nullptr，则所有的信息均不参与计算
        CGRAPH_ASSERT_NOT_NULL(cur);
    }

    for (GraphNode* cur : dependNodes) {
        if (cur == node) {
            continue;        // 本节点无法依赖本节点
        }

        cur->run_before_.insert(node);
        node->dependence_.insert(cur);
    }

    node->left_depend_ = node->dependence_.size();

    CGRAPH_FUNCTION_END
}


/** protected **/
CSTATUS Graphic::checkFinalStatus(int runNodeSize) {
    CGRAPH_FUNCTION_BEGIN

    // 判定是否是所有的节点均被执行1次
    status = (runNodeSize == graph_nodes_.size()) ? STATUS_OK : STATUS_ERR;
    CGRAPH_FUNCTION_CHECK_STATUS

    // 判定是否所有节点均执行到了
    for (GraphNode* node : graph_nodes_) {
        if (false == node->done_) {
            status = STATUS_ERR;
        }

        // 将节点信息复位，以便于开始下一次循环
        node->done_.store(false);
        node->left_depend_ = node->dependence_.size();
    }

    CGRAPH_FUNCTION_END
}

