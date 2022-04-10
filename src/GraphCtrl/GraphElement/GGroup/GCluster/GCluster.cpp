/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCluster.cpp
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/

#include "GCluster.h"

CGRAPH_NAMESPACE_BEGIN

GCluster::GCluster() = default;


GCluster::~GCluster() = default;


GCluster::GCluster(const GCluster& cluster) {
    this->cluster_elements_ = cluster.cluster_elements_;
}


GCluster& GCluster::operator=(const GCluster& cluster) {
    if (this == &cluster) {
        return *this;
    }

    this->cluster_elements_ = cluster.cluster_elements_;
    return *this;
}


CStatus GCluster::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : cluster_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        status = element->fatProcessor(CFunctionType::INIT);
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GCluster::destroy() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : cluster_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        status = element->fatProcessor(CFunctionType::DESTROY);
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GCluster::run() {
    CGRAPH_FUNCTION_BEGIN
    for (GElementPtr element : this->cluster_elements_) {
        CSize elementLoop = element->loop_;
        // element需要被执行loop次
        status = element->fatProcessor(CFunctionType::RUN, elementLoop);
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GCluster::process(bool isMock) {
    CGRAPH_FUNCTION_BEGIN

    status = this->beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    if (likely(!isMock)) {
        // 如果是mock执行，则不进入这里
        for (GElementPtr element : this->cluster_elements_) {
            CSize clusterLoop = element->loop_;
            // cluster 需要被执行loop次
            status = element->fatProcessor(CFunctionType::RUN, clusterLoop);
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    status = this->afterRun();
    CGRAPH_FUNCTION_END
}


CStatus GCluster::beforeRun() {
    CGRAPH_FUNCTION_BEGIN

    this->done_ = false;
    this->left_depend_ = dependence_.size();
    for (GElementPtr element : this->cluster_elements_) {
        status = element->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GCluster::afterRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->cluster_elements_) {
        status = element->afterRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    for (auto& element : this->run_before_) {
        element->left_depend_--;
    }
    this->done_ = true;

    CGRAPH_FUNCTION_END
}


CStatus GCluster::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->cluster_elements_.emplace_back(element);

    CGRAPH_FUNCTION_END
}


CSize GCluster::getElementNum() {
    auto num = cluster_elements_.size();
    return num;
}


CBool GCluster::isElementsDone() {
    /* 所有的element均被执行过，则提示true */
    return std::all_of(cluster_elements_.begin(), cluster_elements_.end(),
                       [](GElementPtr element) {
                           return element->done_;
                       });
}

CGRAPH_NAMESPACE_END