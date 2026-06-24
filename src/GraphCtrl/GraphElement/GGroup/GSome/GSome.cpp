/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSome.inl
@Time: 2023/8/20 11:20
@Desc:
***************************/

#ifndef CGRAPH_GSOME_CPP
#define CGRAPH_GSOME_CPP

#include "GSome.h"

CGRAPH_NAMESPACE_BEGIN

GSome::GSome() {
    element_type_ = GElementType::SOME;
    binding_index_ = CGRAPH_TRIGGER_ALL_THREAD_STRATEGY;
    session_ = URandom<>::generateSession(CGRAPH_STR_SOME);
}


CStatus GSome::addElementEx(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(element)

    /**
     * GSome中插入的元素，必须要有 timeout的设定，如果没有的话，强行写到max值。
     * 用于确保在pipeline run执行完成之前，所有的 element 都会被回收
     */
    if (CGRAPH_DEFAULT_ELEMENT_TIMEOUT == element->timeout_) {
        element->setTimeout(CGRAPH_MAX_BLOCK_TTL, GElementTimeoutStrategy::HOLD_BY_PIPELINE);
    }

    CGRAPH_FUNCTION_END
}


CStatus GSome::run()  {
    CGRAPH_FUNCTION_BEGIN

    wait_num_ = static_cast<CInt>(getWaitNum());
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(wait_num_ > children_.size(),
        "num is bigger than elements size.");
    cur_status_.reset();

    /**
     * 1. 并发的执行，其中的所有逻辑信息
     * 2. 等待 Trigger Num 个element执行完成
     * 3. 将所有未执行完的element 设置为timeout
     * 4. 赋返回值
     */
    for (auto* element : children_) {
        thread_pool_->execute([this, element] {
            {
                const auto& curStatus = element->fatProcessor(CFunctionType::RUN);
                CGRAPH_UNIQUE_LOCK lock(lock_);
                cur_status_ += curStatus;
                if (--wait_num_ <= 0 || cur_status_.isErr()) {
                    cv_.notify_one();
                }
            }
        }, binding_index_);
    }

    CGRAPH_UNIQUE_LOCK lock(lock_);
    cv_.wait(lock, [this] {
        return wait_num_ <= 0 || cur_status_.isErr();
    });

    for (auto* element : children_) {
        if (!element->done_) {
            element->cur_state_.store(GElementState::TIMEOUT, std::memory_order_release);
        }
    }
    status = cur_status_;
    CGRAPH_FUNCTION_END
}


CBool GSome::isSerializable() const {
    return false;    // 情况较为复杂，默认不可以
}


CVoid GSome::dump(std::ostream& oss) {
    dumpElement(oss);
    dumpGroupLabelBegin(oss);
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;style=dashed;\n";    // 蓝色虚线

    for (const auto& element : children_) {
        element->dump(oss);
    }

    dumpGroupLabelEnd(oss);

    for (const auto& element : run_before_) {
        dumpEdge(oss, this, element);
    }
}


CBool GSome::isHold() {
    // 这里固定是不可以 hold的
    return false;
}


CStatus GSome::checkSuitable() {
    CGRAPH_FUNCTION_BEGIN
    status = GElement::checkSuitable();
    CGRAPH_FUNCTION_CHECK_STATUS

    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((CGRAPH_DEFAULT_LOOP_TIMES != loop_), "GSome cannot set loop > 1.")
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(std::any_of(children_.begin(), children_.end(), [](GElementPtr ptr) {
        return !ptr->isAsync();
    }), "GSome contains async node only.")

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSOME_CPP