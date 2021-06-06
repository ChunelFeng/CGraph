/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCluster.cpp
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/

#include "GCluster.h"

GCluster::GCluster() : GElement() {

}

GCluster::~GCluster() = default;

GCluster::GCluster(const GCluster& cluster)  : GElement(cluster) {
    this->cluster_elements_ = cluster.cluster_elements_;
}

GCluster& GCluster::operator=(const GCluster& cluster) {
    if (this == &cluster) {
        return *this;
    }

    this->cluster_elements_ = cluster.cluster_elements_;
    return *this;
}

CSTATUS GCluster::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : cluster_elements_) {
        status = element->init();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CSTATUS GCluster::deinit() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : cluster_elements_) {
        status = element->deinit();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CSTATUS GCluster::run() {
    CGRAPH_FUNCTION_BEGIN
    for (GElementPtr element : this->cluster_elements_) {
        int elementLoop = element->loop_;
        while (elementLoop--) {
            // element需要被执行loop次
            status = element->run();
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GCluster::process(bool isMock) {
    CGRAPH_FUNCTION_BEGIN

    status = this->beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    if (!isMock) {
        // 如果是mock执行，则不进入这里
        for (GElementPtr element : this->cluster_elements_) {
            int clusterLoop = element->loop_;
            while (clusterLoop--) {
                // cluster 需要被执行loop次
                status = element->run();
                CGRAPH_FUNCTION_CHECK_STATUS
            }
        }
    }

    status = this->afterRun();
    CGRAPH_FUNCTION_END
}


CSTATUS GCluster::beforeRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->cluster_elements_) {
        status = element->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    this->done_ = false;
    this->left_depend_ = this->dependence_.size();

    CGRAPH_FUNCTION_END
}

CSTATUS GCluster::afterRun() {
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


CSTATUS GCluster::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)
    CGRAPH_ASSERT_INIT(false)

    this->cluster_elements_.emplace_back(element);

    CGRAPH_FUNCTION_END
}

int GCluster::getElementNum() {
    return this->cluster_elements_.size();
}


bool GCluster::isElementsDone() {
    /* 所有的element均被执行过，则提示true */
    return std::all_of(cluster_elements_.begin(), cluster_elements_.end(),
                       [](const GElementPtr element) {
         return element->done_;
    });
}

