/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemonObject.inl
@Time: 2022/2/2 9:36 下午
@Desc:
***************************/

#ifndef CGRAPH_GDAEMONOBJECT_INL
#define CGRAPH_GDAEMONOBJECT_INL

CGRAPH_NAMESPACE_BEGIN

template <typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GDaemonObject::getGParam(const std::string &key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(pipeline_param_manager_)

    T* ptr = this->pipeline_param_manager_->get<T>(key);
    return ptr;
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GDAEMONOBJECT_INL