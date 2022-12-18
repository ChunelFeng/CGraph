/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemonObject.inl
@Time: 2022/12/18 9:36 下午
@Desc:
***************************/

#ifndef CGRAPH_GDAEMONOBJECT_INL
#define CGRAPH_GDAEMONOBJECT_INL

#include "GDaemonObject.h"

CGRAPH_NAMESPACE_BEGIN

template <typename DParam,
        std::enable_if_t<std::is_base_of<GDaemonParam, DParam>::value, int>>
GDaemonObjectPtr GDaemonObject::setDParam(DParam* param) {
    if (param) {
        CGRAPH_DELETE_PTR(param_)
        param_ = CGRAPH_SAFE_MALLOC_COBJECT(DParam)    // 确保param是最新的
        param_->clone(static_cast<DParam *>(param));
    }

    return this;
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDAEMONOBJECT_INL