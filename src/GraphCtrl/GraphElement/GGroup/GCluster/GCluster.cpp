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
    element_type_ = GElementType::CLUSTER;
    session_ = URandom<>::generateSession(CGRAPH_STR_CLUSTER);
}


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

    this->beforeRun();
    if (likely(!isMock)) {
        // 如果是mock执行，则不进入这里
        for (GElementPtr element : this->group_elements_arr_) {
            status = element->fatProcessor(CFunctionType::RUN);
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    this->afterRun();
    CGRAPH_FUNCTION_END
}


CVoid GCluster::beforeRun() {
    this->done_ = false;
    this->left_depend_ = dependence_.size();
    for (GElementPtr element : this->group_elements_arr_) {
        element->beforeRun();
    }
}


CVoid GCluster::afterRun() {
    for (GElementPtr element : this->group_elements_arr_) {
        element->afterRun();
    }

    for (auto& element : this->run_before_) {
        element->left_depend_--;
    }
    this->done_ = true;
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


CBool GCluster::isDone() {
    /* 所有的element均被执行过，则提示true */
    return std::all_of(group_elements_arr_.begin(), group_elements_arr_.end(),
                       [](GElementPtr element) {
        return element->done_;
    });
}


CIndex GCluster::getBindingIndex() {
    if (group_elements_arr_.empty()) {
        // 防止未注入的情况发生
        return CGRAPH_DEFAULT_BINDING_INDEX;
    }

    // 实际就是返回第一个元素的 binding index 值
    return group_elements_arr_[0]->getBindingIndex();
}


CVoid GCluster::dump(std::ostream& oss) {
    dumpElement(oss);
    dumpGroupLabelBegin(oss);
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;\n";

    GElementPtr pre = nullptr;
    for (auto i = 0; i < group_elements_arr_.size(); i++) {
        const auto& element = group_elements_arr_[i];
        element->dump(oss);

        if (0 != i) {
            // 如果不是开头的位置，则划线关联
            dumpEdge(oss, pre, element);
        }
        pre = element;
    }

    dumpGroupLabelEnd(oss);

    for (const auto& node : run_before_) {
        dumpEdge(oss, this, node);
    }
}

CGRAPH_NAMESPACE_END