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
CStatus GSome<TriggerNum>::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    if (GElementType::ASYNC_NODE != element->element_type_) {
        group_elements_arr_.clear();    // 如果有错误的情况，就直接清空本地的内容
        CGRAPH_RETURN_ERROR_STATUS("GSome can insert async node only current.")
    }

    group_elements_arr_.template emplace_back(element);
    CGRAPH_FUNCTION_END
}


template<CSize TriggerNum>
GSome<TriggerNum>::GSome() {
    element_type_ = GElementType::SOME;
    session_ = URandom<>::generateSession(CGRAPH_STR_SOME);
}


template<CSize TriggerNum>
CStatus GSome<TriggerNum>::run()  {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)
    left_num_.store(TriggerNum);    // 还剩n个，就完成当前GSome的执行逻辑
    cur_status_ = CStatus();

    for (auto* element : group_elements_arr_) {
        process((GAsyncNodePtr)element);
    }

    CGRAPH_UNIQUE_LOCK lock(lock_);
    cv_.wait(lock, [this] {
        return left_num_ <= 0 || cur_status_.isErr();
    });

    if (!cur_status_.isOK()) {
        status = cur_status_;    // 出错的话，赋值到外部去，让上游知道。
    }
    CGRAPH_FUNCTION_END
}


template<CSize TriggerNum>
CVoid GSome<TriggerNum>::process(GAsyncNodePtr ptr) {
    // 这里的内容，仅可能为 GAsyncNode 的子类的信息
    const auto& exec = [this, ptr] {
        {
            CGRAPH_UNIQUE_LOCK lock(lock_);
            cur_status_ += ptr->run();    // 开始异步执行起来了
        }
        if (cur_status_.isOK()) {
            cur_status_ += ptr->getResult();
        }
        left_num_--;
        cv_.notify_one();
    };

    thread_pool_->commit(exec);
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