/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.inl
@Time: 2021/6/1 10:13 下午
@Desc:
***************************/

#ifndef CGRAPH_GELEMENT_INL
#define CGRAPH_GELEMENT_INL

template<typename T>
CSTATUS GElement::createGParam(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(this->param_manager_)

    status = this->param_manager_->create<T>(key);
    CGRAPH_FUNCTION_END
}


template<typename T>
T* GElement::getGParam(const std::string& key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->param_manager_)

    T* ptr = dynamic_cast<T *>(this->param_manager_->get(key));
    return ptr;
}


template <typename T, std::enable_if_t<std::is_base_of_v<GAspect, T>, int>>
GElementPtr GElement::addAspect() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(aspect_manager_)

    GAspectPtr aspect = CGRAPH_SAFE_MALLOC_COBJECT(T);
    aspect->setName(this->getName());
    aspect_manager_->addAspect(aspect);
    return this;
}


template<typename T1, typename T2,
        std::enable_if_t<std::is_base_of_v<GAspect, T1>, int>,
        std::enable_if_t<std::is_base_of_v<GAspect, T2>, int>>
GElement* GElement::addAspect() {
    return this->template addAspect<T1>()->template addAspect<T2>();
}


#endif // CGRAPH_GELEMENT_INL
