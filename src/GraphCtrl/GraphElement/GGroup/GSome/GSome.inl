/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSome.inl
@Time: 2023/8/20 11:20
@Desc:
***************************/

#ifndef CGRAPH_GSOME_INL
#define CGRAPH_GSOME_INL

#include "GSome.h"

CGRAPH_NAMESPACE_BEGIN

template<CSize TriggerNum>
GSome<TriggerNum>::GSome() {
    element_type_ = GElementType::SOME;
    session_ = URandom<>::generateSession(CGRAPH_STR_SOME);
}


template<CSize TriggerNum>
CStatus GSome<TriggerNum>::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    group_elements_arr_.emplace_back(element);
    CGRAPH_FUNCTION_END
}


template<CSize TriggerNum>
CStatus GSome<TriggerNum>::run()  {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((0 == TriggerNum), "trigger num cannot set 0.")
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((group_elements_arr_.size() < TriggerNum),     \
                                            "this GSome need at least [" + std::to_string(TriggerNum) + "] element")

    left_num_ = TriggerNum;    // 还剩n个，就完成当前GSome的执行逻辑
    cur_status_ = CStatus();

    for (auto* element : group_elements_arr_) {
        thread_pool_->commit([this, element] {
            auto curStatus = element->fatProcessor(CFunctionType::RUN);
            {
                CGRAPH_UNIQUE_LOCK lock(lock_);
                cur_status_ += curStatus;
                left_num_--;
            }
            cv_.notify_one();
        }, CGRAPH_POOL_TASK_STRATEGY);
    }

    CGRAPH_UNIQUE_LOCK lock(lock_);
    cv_.wait(lock, [this] {
        return left_num_ == 0 || cur_status_.isErr();
    });

    status = cur_status_;
    CGRAPH_FUNCTION_END
}


template<CSize TriggerNum>
CBool GSome<TriggerNum>::isSerializable() {
    return false;    // 情况较为复杂，默认不可以
}


template<CSize TriggerNum>
CVoid GSome<TriggerNum>::dump(std::ostream& oss) {
    dumpElement(oss);
    dumpGroupLabelBegin(oss);
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;style=dashed;\n";    // 蓝色虚线

    for (const auto& element : group_elements_arr_) {
        element->dump(oss);
    }

    dumpGroupLabelEnd(oss);

    for (const auto& element : run_before_) {
        dumpEdge(oss, this, element);
    }
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSOME_INL