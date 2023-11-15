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
    trigger_times_ = 0;
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
    trigger_times_ = 0;
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
        oss << "(loop=" << this->loop_ << ")";
    }

    dumpPerfInfo(oss);
    oss << "\";\n";
}


CVoid GGroup::dumpGroupLabelEnd(std::ostream& oss) {
    oss << "}\n";
}


CBool GGroup::isSerializable() {
    /**
     * 针对group的情况，应该是所有在其中的element 都是可以串行的，才认定为可串行
     * 但是在 region和 multiCondition中，有针对性的判断
     */
    return std::all_of(group_elements_arr_.begin(), group_elements_arr_.end(),
                       [](GElementPtr element) {
        return element->isSerializable();
    });
}


CBool GGroup::isRegistered() {
    // 被注册的过group，这两个manager是不会为空的
    return (nullptr != param_manager_) && (nullptr != event_manager_);
}

CGRAPH_NAMESPACE_END