/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStage.inl
@Time: 2024/12/30 9:46 下午
@Desc:
***************************/

#ifndef CGRAPH_GSTAGE_INL
#define CGRAPH_GSTAGE_INL

#include "GStage.h"

CGRAPH_NAMESPACE_BEGIN

template <typename T,
        c_enable_if_t<std::is_base_of<GStageParam, T>::value, int>>
GStagePtr GStage::setSParam(T* param) {
    if (param) {
        CGRAPH_DELETE_PTR(param_);
        param_ = CGRAPH_SAFE_MALLOC_COBJECT(T);
        param_->clone(static_cast<T *>(param));
    }

    return this;
}

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTAGE_INL