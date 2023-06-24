/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementRepository.cpp
@Time: 2023/6/24 17:51
@Desc: 
***************************/

#include "GElementRepository.h"

CGRAPH_NAMESPACE_BEGIN

CVoid GElementRepository::insert(GElementPtr ptr) {
    if (nullptr != ptr) {
        elements_.insert(ptr);
    }
}


CBool GElementRepository::find(GElementPtr ptr) {
    if (nullptr == ptr) {
        return false;
    }
    return elements_.find(ptr) != elements_.end();
}


GElementRepositoryPtr GElementRepository::setThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(ptr)
    for (auto& cur : this->elements_) {
        cur->setThreadPool(ptr);
    }
    return this;
}


CStatus GElementRepository::setup() {
    CGRAPH_FUNCTION_BEGIN
    if (elements_.empty()
        || GElementState::NORMAL != (*elements_.begin())->cur_state_.load()) {
        return status;
    }

    status = pushAllState(GElementState::NORMAL);
    CGRAPH_FUNCTION_END
}


CStatus GElementRepository::pushAllState(GElementState state) {
    CGRAPH_FUNCTION_BEGIN

    for (auto* cur : elements_) {
        cur->cur_state_.store(state);
        if (GElementState::YIELD != state) {
            // 目前仅非yield状态，需要切换的。如果一直处于 yield状态，是不需要被通知的
            cur->yield_cv_.notify_one();
        }
    }
    CGRAPH_FUNCTION_END
}


GElementRepository::~GElementRepository() {
    // 删除所有内部的element信息
    for (GElementPtr element : elements_) {
        CGRAPH_DELETE_PTR(element)
    }
}


CStatus GElementRepository::run() {
   CGRAPH_NO_SUPPORT
}

CGRAPH_NAMESPACE_END