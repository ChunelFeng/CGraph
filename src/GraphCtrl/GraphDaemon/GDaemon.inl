/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemon.inl
@Time: 2022/2/2 9:54 下午
@Desc:
***************************/

#ifndef CGRAPH_GDAEMON_INL
#define CGRAPH_GDAEMON_INL

CGRAPH_NAMESPACE_BEGIN

template <typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GDaemon::getGParam(const std::string &key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(pipeline_param_manager_)

    T* ptr = this->pipeline_param_manager_->get<T>(key);
    return ptr;
}

template <typename DParam,
        std::enable_if_t<std::is_base_of<GDaemonParam, DParam>::value, int>>
GDaemonPtr GDaemon::setDParam(DParam* param) {
    if (param) {
        CGRAPH_DELETE_PTR(param_)
        param_ = CGRAPH_SAFE_MALLOC_COBJECT(DParam)    // 确保param是最新的
        param_->clone(static_cast<DParam *>(param));
    }

    return this;
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDAEMON_INL