/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMultiParallelCondition.cpp
@Time: 2023/6/16 00:05
@Desc: 
***************************/

#include "GMultiParallelCondition.h"

CGRAPH_NAMESPACE_BEGIN

GMultiParallelCondition::GMultiParallelCondition() {
    element_type_ = GElementType::PARALLEL_CONDITION;
    session_ = URandom<>::generateSession(CGRAPH_STR_PARALLEL_CONDITION);
}


CStatus GMultiParallelCondition::run() {
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


CIndex GMultiParallelCondition::choose() {
    CGRAPH_THROW_EXCEPTION("GMultiParallelCondition no need choose function")
    return 0;
}

CGRAPH_NAMESPACE_END
