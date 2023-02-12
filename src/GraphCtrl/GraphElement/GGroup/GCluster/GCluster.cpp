/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCluster.cpp
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/

#include "GCluster.h"

CGRAPH_NAMESPACE_BEGIN

GCluster::GCluster() {
    element_type_ = (0x0010 << 1) | 0x0010;
}


GCluster::~GCluster() = default;


GCluster::GCluster(const GCluster& cluster) {
    this->group_elements_arr_ = cluster.group_elements_arr_;
}


GCluster& GCluster::operator=(const GCluster& cluster) {
    if (this == &cluster) {
        return *this;
    }

    this->group_elements_arr_ = cluster.group_elements_arr_;
    return *this;
}


CStatus GCluster::run() {
    CGRAPH_FUNCTION_BEGIN
    for (GElementPtr element : this->group_elements_arr_) {
        status = element->fatProcessor(CFunctionType::RUN);
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GCluster::process(CBool isMock) {
    CGRAPH_FUNCTION_BEGIN

    status = this->beforeRun();
    CGRAPH_FUNCTION_CHECK_STATUS

    if (likely(!isMock)) {
        // 如果是mock执行，则不进入这里
        for (GElementPtr element : this->group_elements_arr_) {
            status = element->fatProcessor(CFunctionType::RUN);
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
    for (GElementPtr element : this->group_elements_arr_) {
        status = element->beforeRun();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CStatus GCluster::afterRun() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : this->group_elements_arr_) {
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

    this->group_elements_arr_.emplace_back(element);

    CGRAPH_FUNCTION_END
}


CSize GCluster::getElementNum() {
    auto num = group_elements_arr_.size();
    return num;
}


CBool GCluster::isClusterDone() {
    /* 所有的element均被执行过，则提示true */
    return std::all_of(group_elements_arr_.begin(), group_elements_arr_.end(),
                       [](GElementPtr element) {
                           return element->done_;
                       });
}


CVoid GCluster::dump(std::ostream& oss) {
    dumpElement(oss);
    oss << "subgraph ";
    oss << "cluster_p" << this;
    oss << " {\nlabel=\"";
    if (name_.empty()) oss << 'p' << this;
    else oss << name_;
    if (this->loop_ > 1) {
        oss << " loop=" << this->loop_;
    }
    oss << "\";\n";
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;\n";

    GElementPtr pre = nullptr;
    for (size_t idx = 0; idx < group_elements_arr_.size(); idx++) {
        const auto& element = group_elements_arr_[idx];
        element->dump(oss);

        if (idx != 0) {
            dumpEdge(oss, pre, element);
        }
        pre = element;
    }

    oss << "}\n";

    for (const auto& node : run_before_) {
        dumpEdge(oss, this, node);
    }
}

CGRAPH_NAMESPACE_END