/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMultiCondition.cpp
@Time: 2023/6/17 19:21
@Desc: 
***************************/

#ifndef CGRAPH_GMULTICONDITION_INL
#define CGRAPH_GMULTICONDITION_INL

#include "GMultiCondition.h"

CGRAPH_NAMESPACE_BEGIN

template<GMultiConditionType type>
GMultiCondition<type>::GMultiCondition() {
    element_type_ = GElementType::MULTI_CONDITION;
    session_ = URandom<>::generateSession(CGRAPH_STR_MULTI_CONDITION);
}


template<GMultiConditionType type>
CStatus GMultiCondition<type>::run() {
    CGRAPH_FUNCTION_BEGIN

    switch (type) {
        case GMultiConditionType::SERIAL:
            status = serialRun();
            break;
        case GMultiConditionType::PARALLEL:
            status = parallelRun();
            break;
        default:
            CGRAPH_RETURN_ERROR_STATUS("unknown multi condition type")
    }

    CGRAPH_FUNCTION_END
}


template<GMultiConditionType type>
CStatus GMultiCondition<type>::serialRun() {
    CGRAPH_FUNCTION_BEGIN

    for (auto cur: this->group_elements_arr_) {
        if (cur->isMatch()) {
            // 仅依次执行 match 的逻辑
            status = cur->fatProcessor(CFunctionType::RUN);
            CGRAPH_FUNCTION_CHECK_STATUS
        }
    }

    CGRAPH_FUNCTION_END
}


template<GMultiConditionType type>
CStatus GMultiCondition<type>::parallelRun() {
    CGRAPH_FUNCTION_BEGIN
    std::vector<std::future<CStatus> > futures;
    for (GElementPtr cur : this->group_elements_arr_) {
        if (!cur->isMatch()) {
            continue;    // 不满足条件，则不执行
        }

        futures.emplace_back(this->thread_pool_->commit([cur] {
            return cur->fatProcessor(CFunctionType::RUN);
        }, cur->getBindingIndex()));
    }

    for (auto& fut: futures) {
        /**
         * 如果有异常值的化，也等待所有内容计算完成后，统一返回
         * 暂时没有处理超时的情况。预计今后会统一处理
         */
        status += fut.get();
    }

    CGRAPH_FUNCTION_END
}


template<GMultiConditionType type>
CIndex GMultiCondition<type>::choose() {
    // GMultiCondition 是不可能执行到 choose()方法的。这里直接抛出异常好了
    CGRAPH_THROW_EXCEPTION("GMultiCondition no need choose function")
    return 0;
}


template<GMultiConditionType type>
CBool GMultiCondition<type>::isSerializable() {
    if (GMultiConditionType::PARALLEL == type && group_elements_arr_.size() > 1) {
        /**
         * 如果是PARALLEL模式的话，并且其中的元素个数大于1，则一定不可以串行执行
         * PARALLEL模式中，仅有一个元素的情况，和 SERIAL模式的判断方式一样，
         * 均为判断其中所有的element是否可以并行
         * 故放在下面的条件中判断了
         */
        return false;
    }

    return GGroup::isSerializable();
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMULTICONDITION_INL
