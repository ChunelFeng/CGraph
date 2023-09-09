/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GFence.cpp
@Time: 2023/9/6 21:54
@Desc: 
***************************/

#include "GFence.h"

CGRAPH_NAMESPACE_BEGIN

GFence::GFence() {
    element_type_ = GElementType::FENCE;
    session_ = URandom<>::generateSession(CGRAPH_STR_FENCE);
}


GFencePtr GFence::waitGElement(GElementPtr element) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element)
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(!element->isAsync(),     \
    "fence can add async element only. please set timeout value for [" + element->getName() + "] if you need.")

    fence_elements_.insert(element);
    return this;
}


GFencePtr GFence::waitGElements(const std::set<GElementPtr>& elements) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    for (auto* element : elements) {
        waitGElement(element);
    }
    return this;
}


CVoid GFence::clear() {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    fence_elements_.clear();
}


CStatus GFence::checkSuitable() {
    CGRAPH_FUNCTION_BEGIN
    for (auto* element : fence_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!element->isAsync(),     \
        "[" + element->name_ + "] is not async node. please set timeout value if you want to fence it")
    }

    CGRAPH_FUNCTION_END
}


CVoid GFence::dumpElement(std::ostream& oss) {
    dumpElementHeader(oss);
    dumpPerfInfo(oss);
    oss << "\", shape=box]\n";
}


CStatus GFence::run() {
    CGRAPH_FUNCTION_BEGIN

    for (auto* element : fence_elements_) {
        // 等待全部到达之后，判断错误信息
        status += element->getAsyncResult();
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END