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


CBool GElementRepository::find(GElementPtr ptr) const {
    if (nullptr == ptr) {
        return false;
    }
    return elements_.find(ptr) != elements_.end();
}


GElementRepositoryPtr GElementRepository::setThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(ptr)
    for (auto& cur : this->elements_) {
        cur->setThreadPool(ptr);
    }
    return this;
}


CStatus GElementRepository::setup() {
    CGRAPH_FUNCTION_BEGIN
    // 一旦执行，全部设置为 normal状态
    if (GElementState::NORMAL == cur_state_) {
        return status;
    }

    status = pushAllState(GElementState::NORMAL);
    CGRAPH_FUNCTION_END
}


CStatus GElementRepository::pushAllState(const GElementState& state) {
    CGRAPH_FUNCTION_BEGIN

    cur_state_ = state;    // 记录当前的状态信息
    for (auto* cur : elements_) {
        cur->cur_state_.store(state);
        if (GElementState::YIELD != state) {
            // 目前仅非yield状态，需要切换的。如果一直处于 yield状态，是不需要被通知的
            cur->yield_cv_.notify_one();
        }
    }
    CGRAPH_FUNCTION_END
}


CBool GElementRepository::isCancelState() const {
    /**
     * 因为每次执行的时候，都需要判断一下这个状态是否为 cancel
     * 且理论上不会出现多线程问题
     * 故这一层的 cur_state_ 就不设置为atomic类型的了
     */
    return GElementState::CANCEL == cur_state_;
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