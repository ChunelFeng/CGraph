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

template <typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GDaemonObject::getGParam(const std::string &key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(pipeline_param_manager_)

    T* ptr = this->pipeline_param_manager_->get<T>(key);
    return ptr;
}


template <typename DParam,
        std::enable_if_t<std::is_base_of<GDaemonParam, DParam>::value, int>>
GDaemonObject* GDaemonObject::setDParam(DParam* param) {
    if (param) {
        CGRAPH_DELETE_PTR(param_)
        param_ = CGRAPH_SAFE_MALLOC_COBJECT(DParam)    // 确保param是最新的
        param_->clone(static_cast<DParam *>(param));
    }

    return this;
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDAEMONOBJECT_INL