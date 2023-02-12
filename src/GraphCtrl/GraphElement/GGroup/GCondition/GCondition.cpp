/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCondition.cpp
@Time: 2021/6/19 5:00 下午
@Desc: 
***************************/

#include "GCondition.h"

CGRAPH_NAMESPACE_BEGIN


GCondition::GCondition() {
    element_type_ = (0x0010 << 3) | 0x0010;
}


CStatus GCondition::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->group_elements_arr_.emplace_back(element);
    CGRAPH_FUNCTION_END
}


CStatus GCondition::run() {
    CGRAPH_FUNCTION_BEGIN

    CIndex index = this->choose();
    if (GROUP_LAST_ELEMENT_INDEX == index
        && !this->group_elements_arr_.empty()) {
        // 如果返回-1，则直接执行最后一个条件（模仿default功能）
        auto element = group_elements_arr_.back();
        status = element->fatProcessor(CFunctionType::RUN);
    } else if (0 <= index && index < (CIndex)group_elements_arr_.size()) {
        // 如果返回的内容，在元素范围之内，则直接执行元素的内容。不在的话，则不执行任何操作，直接返回正确状态
        auto element = group_elements_arr_[index];
        status = element->fatProcessor(CFunctionType::RUN);
    }

    CGRAPH_FUNCTION_END
}


CSize GCondition::getRange() const {
    return group_elements_arr_.size();
}


CVoid GCondition::dump(std::ostream& oss) {
    dumpElement(oss);
    oss << "subgraph ";
    oss << "cluster_p" << this;
    oss << " {\nlabel=\"";
    if (name_.empty()) oss << 'p' << this;
    else oss << name_;
    oss << "\";\n";
    oss << 'p' << this << "[shape=diamond];\n";
    oss << "color=blue;\n";

    for (size_t idx = 0; idx < group_elements_arr_.size(); ++idx) {
        const auto& element = group_elements_arr_[idx];
        element->dump(oss);

        std::string label = "[label=\"" + std::to_string(idx) + "\"]";
        dumpEdge(oss, this, element, label);
    }

    oss << "}\n";

    for (const auto& node : run_before_) {
        dumpEdge(oss, this, node);
    }
}

CGRAPH_NAMESPACE_END
