/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStaticEngine.cpp
@Time: 2022/12/11 16:43
@Desc: 
***************************/

#include "GStaticEngine.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GStaticEngine::setup(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    status = mark(elements);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = analyse(elements);
    CGRAPH_FUNCTION_END
}


CStatus GStaticEngine::mark(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    total_element_size_ = (CUint)elements.size();    // 仅赋值一次，不会改变了

    /**
     * 认定图可以连通的判定条件：
     * 1，当前元素仅有一个依赖
     * 2，当前元素依赖的节点，只有一个后继
     * 3，当前元素的依赖的后继，仍是当前节点
     * 4，前后元素绑定机制是一样的
     */
    for (GElementPtr element : elements) {
        if (1 == element->dependence_.size()
            && 1 == (*element->dependence_.begin())->run_before_.size()
            && (*(element->dependence_.begin()))->run_before_.find(element) != (*(element->dependence_.begin()))->run_before_.end()
            && element->getBindingIndex() == (*(element->dependence_.begin()))->getBindingIndex()) {
            element->linkable_ = true;
        }
    }

    CGRAPH_FUNCTION_END
}


CStatus GStaticEngine::analyse(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    run_element_size_ = 0;
    para_cluster_arrs_.clear();

    GClusterArr curClusterArr;    // 记录每一层，可以并行的逻辑
    for (GElementPtr element : elements) {
        if (!element->isRunnable() || element->isLinkable()) {
            continue;
        }

        GCluster curCluster;
        GElementPtr curElement = element;
        curCluster.addElement(curElement);

        /* 将linkable的节点，统一放到一个cluster中 */
        while (1 == curElement->run_before_.size()
               && (*curElement->run_before_.begin())->isLinkable()) {
            // 将下一个放到cluster中处理
            curElement = (*curElement->run_before_.begin());
            curCluster.addElement(curElement);
        }
        curClusterArr.emplace_back(curCluster);
    }
    para_cluster_arrs_.emplace_back(curClusterArr);

    GClusterArr runnableClusterArr;
    while (!curClusterArr.empty() && run_element_size_ <= total_element_size_) {
        runnableClusterArr = curClusterArr;
        curClusterArr.clear();

        for (GClusterRef cluster : runnableClusterArr) {
            status = cluster.process(true);    // 不执行run方法的process
            CGRAPH_FUNCTION_CHECK_STATUS
        }
        run_element_size_ += (CUint)runnableClusterArr.size();

        GElementPtrSet duplications;
        for (GClusterRef cluster : runnableClusterArr) {
            for (GElementPtr element : cluster.group_elements_arr_) {
                for (GElementPtr cur : element->run_before_) {
                    /**
                     * 判断element是否需要被加入
                     * 1，该元素是可以执行的
                     * 2，该元素本次循环是第一次被遍历
                     */
                    if (cur->isRunnable() && duplications.end() == duplications.find(cur)) {
                        GCluster curCluster;
                        GElementPtr curElement = cur;
                        curCluster.addElement(curElement);
                        duplications.insert(curElement);

                        while (1 == curElement->run_before_.size()
                               && (*curElement->run_before_.begin())->isLinkable()) {
                            curElement = (*curElement->run_before_.begin());
                            curCluster.addElement(curElement);
                            duplications.insert(curElement);
                        }
                        curClusterArr.emplace_back(curCluster);
                    }
                }
            }
        }

        /* 为空的话，直接退出循环；不为空的话，放入para信息中 */
        if (!curClusterArr.empty()) {
            para_cluster_arrs_.emplace_back(curClusterArr);
        }
    }

    CGRAPH_FUNCTION_END
}


CStatus GStaticEngine::run() {
    CGRAPH_FUNCTION_BEGIN
    run_element_size_ = 0;    // 每次执行的时候，记录执行了多少个element信息
    std::vector<std::future<CStatus> > futures;

    for (GClusterArrRef clusterArr : para_cluster_arrs_) {
        futures.clear();

        /** 将分解后的pipeline信息，以cluster为维度，放入线程池依次执行 */
        for (GClusterRef cluster : clusterArr) {
            futures.emplace_back(thread_pool_->commit([&cluster] {
                return cluster.process(false);
            }, calcIndex(&cluster)));

            run_element_size_ += cluster.getElementNum();
        }

        for (auto& fut : futures) {
            status += fut.get();
        }
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GStaticEngine::afterRunCheck() {
    CGRAPH_FUNCTION_BEGIN
    /* 验证是否所有的内容均被执行过 */
    if (run_element_size_ != total_element_size_) {
        const std::string& errInfo = "run size = [" + std::to_string(run_element_size_)
                                      + "], total size = [" + std::to_string(total_element_size_) + "] ...";
        CGRAPH_RETURN_ERROR_STATUS("static engine run element size check failed, " + errInfo)
    }

    /* 需要验证每个cluster里的每个内容是否被执行过一次 */
    for (GClusterArrRef clusterArr : para_cluster_arrs_) {
        for (GClusterRef cluster : clusterArr) {
            if (!cluster.isDone()) {
                CGRAPH_RETURN_ERROR_STATUS("static engine run done status check failed...");
            }
        }
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END