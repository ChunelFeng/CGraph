/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamManager.inl
@Time: 2021/6/11 6:59 下午
@Desc:
***************************/

#ifndef CGRAPH_GPARAMMANAGER_INL
#define CGRAPH_GPARAMMANAGER_INL

CGRAPH_NAMESPACE_BEGIN

template<typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
CStatus GParamManager::create(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    auto result = params_map_.find(key);
    if (result != params_map_.end()) {
        /* 如果是重复创建，则返回ok；非重复创建（类型不同）则返回err */
        auto param = result->second;
        return (typeid(*param).name() == typeid(T).name()) ? CStatus() : CStatus("create param duplicate");
    }

    {
        CGRAPH_LOCK_GUARD lock(this->lock_);
        T* ptr = CGRAPH_SAFE_MALLOC_COBJECT(T)
        params_map_.insert(std::pair<std::string, T*>(key, ptr));
    }

    CGRAPH_FUNCTION_END
}


template<typename T,
        std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GParamManager::get(const std::string& key) {
    auto result = params_map_.find(key);
    if (result == params_map_.end()) {
        return nullptr;
    }

    return dynamic_cast<T *>(result->second);
}


template<typename T, std::enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GParamManager::getWithNoEmpty(const std::string& key) {
    auto* param = get<T>(key);
    if (nullptr == param) {
        CGRAPH_THROW_EXCEPTION("param [" + key + "] is null")
    }
    return param;
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GPARAMMANAGER_INL