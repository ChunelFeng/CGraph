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


CStatus GCluster::run() {
    CGRAPH_FUNCTION_BEGIN
    for (GElementPtr element : this->children_) {
        status = element->fatProcessor(CFunctionType::RUN);
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    CGRAPH_FUNCTION_END
}


CVoid GCluster::dump(std::ostream& oss) {
    dumpElement(oss);
    dumpGroupLabelBegin(oss);
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;\n";

    GElementPtr pre = nullptr;
    for (CSize i = 0; i < children_.size(); i++) {
        const auto& element = children_[i];
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


CBool GCluster::isSeparate(GElementCPtr a, GElementCPtr b) const {
    return true;
}

CGRAPH_NAMESPACE_END