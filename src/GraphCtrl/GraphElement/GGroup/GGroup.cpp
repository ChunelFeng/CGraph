/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GGroup.cpp
@Time: 2022/10/16 01:06
@Desc: 
***************************/

#include "GGroup.h"

CGRAPH_NAMESPACE_BEGIN

GGroup::GGroup() {
    element_type_ = GElementType::GROUP;
}

CStatus GGroup::init() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : group_elements_arr_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        status += element->fatProcessor(CFunctionType::INIT);
    }
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GGroup::destroy() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : group_elements_arr_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        status += element->fatProcessor(CFunctionType::DESTROY);
    }
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CVoid GGroup::dumpGroupLabelBegin(std::ostream& oss) {
    oss << "subgraph ";
    oss << "cluster_p" << this;    // cluster_ 是 graphviz的关键字，和CGraph中GCluster逻辑无关
    oss << " {\nlabel=\"";

    if (name_.empty()) {
        oss << 'p' << this;
    } else {
        oss << name_;
    }

    if (this->loop_ > 1) {
        oss << " loop=" << this->loop_;
    }
    oss << "\";\n";
}


CVoid GGroup::dumpGroupLabelEnd(std::ostream& oss) {
    oss << "}\n";
}

CGRAPH_NAMESPACE_END