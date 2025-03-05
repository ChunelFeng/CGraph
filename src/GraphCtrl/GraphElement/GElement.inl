/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GNode.inl
@Time: 2021/6/1 10:13 下午
@Desc:
***************************/

#ifndef CGRAPH_GELEMENT_INL
#define CGRAPH_GELEMENT_INL

#include <typeinfo>

#include "GElement.h"

CGRAPH_NAMESPACE_BEGIN

template<typename TAspect, typename TParam,
        c_enable_if_t<std::is_base_of<GAspect, TAspect>::value, int>,
        c_enable_if_t<std::is_base_of<GAspectParam, TParam>::value, int> >
GElementPtr GElement::addGAspect(TParam* param) {
    if (!aspect_manager_) {
        /** 采用懒加载的方式执行，这里不会有并发问题，故不需要采用单例模式了 */
        aspect_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GAspectManager)
    }

    GAspectPtr aspect = CGRAPH_SAFE_MALLOC_COBJECT(TAspect)
    aspect->setAParam<TParam>(param);
    // 为了适配 python版本，所有的aspect的信息填充，放到 pipeline 初始化之前的瞬间，统一做掉了
    aspect_manager_->add(aspect);
    return this;
}


template<typename TAspect, typename ...Args,
        c_enable_if_t<std::is_base_of<GTemplateAspect<Args...>, TAspect>::value, int>>
GElementPtr GElement::addGAspect(Args... args) {
    if (!aspect_manager_) {
        aspect_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GAspectManager)
    }

    auto aspect = CAllocator::safeMallocTemplateCObject<TAspect>(std::forward<Args>(args)...);
    aspect_manager_->add(aspect);
    return this;
}


template<typename T,
        c_enable_if_t<std::is_base_of<GElementParam, T>::value, int> >
GElementPtr GElement::addEParam(const std::string& key, T* param) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param)
    if (local_params_.end() != local_params_.find(key)) {
        // 如果重复同名key信息，则删除原先的内容
        CGRAPH_DELETE_PTR(local_params_[key]);
    }
    T* cur = CGRAPH_SAFE_MALLOC_COBJECT(T);
    cur->clone(param);

    local_params_[key] = cur;
    return this;
}


template<typename T,
        c_enable_if_t<std::is_base_of<GElementParam, T>::value, int>>
T* GElement::getEParam(const std::string& key) {
    auto iter = local_params_.find(key);
    if (iter == local_params_.end()) {
        return nullptr;
    }

    auto param = iter->second;
    return likely(typeid(T) == typeid(*param)) ? static_cast<T *>(param) : nullptr;
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GELEMENT_INL
