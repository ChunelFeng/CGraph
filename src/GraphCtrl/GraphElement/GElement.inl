/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.inl
@Time: 2021/6/1 10:13 下午
@Desc:
***************************/

#ifndef CGRAPH_GELEMENT_INL
#define CGRAPH_GELEMENT_INL

CGRAPH_NAMESPACE_BEGIN

template<typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int> >
CStatus GElement::createGParam(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(this->param_manager_)

    status = this->param_manager_->create<T>(key);
    CGRAPH_FUNCTION_END
}


template<typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int> >
T* GElement::getGParam(const std::string& key) {
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(this->param_manager_)

    T* ptr = this->param_manager_->get<T>(key);
    return ptr;
}


template<typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int> >
T* GElement::getGParamWithNoEmpty(const std::string& key) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(this->param_manager_)
    return this->param_manager_->getWithNoEmpty<T>(key);
}


template<typename TAspect, typename TParam,
        std::enable_if_t<std::is_base_of<GAspect, TAspect>::value, int>,
        std::enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int> >
GElementPtr GElement::addGAspect(TParam* param) {
    if (!aspect_manager_) {
        /** 采用懒加载的方式执行，这里不会有并发问题，故不需要采用单例模式了 */
        aspect_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GAspectManager)
    }

    GAspectPtr aspect = CGRAPH_SAFE_MALLOC_COBJECT(TAspect)
    aspect->setName(this->getName())
          ->setAParam<TParam>(param);
    aspect->setGParamManager(this->param_manager_);
    aspect_manager_->add(aspect);
    return this;
}


template<typename TAspect, typename ...Args,
        std::enable_if_t<std::is_base_of<GTemplateAspect<Args...>, TAspect>::value, int>>
GElement* GElement::addGAspect(Args... args) {
    if (!aspect_manager_) {
        aspect_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GAspectManager)
    }

    auto aspect = UAllocator::safeMallocTemplateCObject<TAspect>(std::forward<Args>(args)...);
    aspect->setName(this->getName());
    aspect->setGParamManager(this->param_manager_);
    aspect_manager_->add(aspect);
    return this;
}


template<typename T,
        std::enable_if_t<std::is_base_of<GElementParam, T>::value, int> >
GElementPtr GElement::addEParam(const std::string& key, T* param) {
    CGRAPH_ASSERT_INIT_RETURN_NULL(false)
    CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(param)
    if (local_params_.end() != local_params_.find(key)) {
        // 如果重复同名key信息，则删除原先的内容
        CGRAPH_DELETE_PTR(local_params_[key]);
    }
    T* cur = CGRAPH_SAFE_MALLOC_COBJECT(T);
    cur->clone(param);

    local_params_[key] = cur;    // 写入其中
    return this;
}


template<typename T,
        std::enable_if_t<std::is_base_of<GElementParam, T>::value, int> >
T* GElement::getEParam(const std::string& key) {
    auto iter = local_params_.find(key);
    return dynamic_cast<T *>((iter != local_params_.end()) ? iter->second : nullptr);
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GELEMENT_INL
