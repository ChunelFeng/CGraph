
#ifndef CGRAPH_GPARAMMANAGER_INL
#define CGRAPH_GPARAMMANAGER_INL

template<typename T>
CSTATUS GParamManager::create(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    auto result = params_map_.find(key);
    if (result != params_map_.end()) {
        /* 如果是重复创建，则返回ok；非重复创建（类型不同）则返回err */
        auto param = result->second;
        return (typeid(*param).name() == typeid(T).name()) ? STATUS_OK : STATUS_ERR;
    }

    CGRAPH_WRITE_LOCK wLock(this->lock_);
    T* ptr = CGRAPH_SAFE_MALLOC_COBJECT(T);

    params_map_.insert(std::pair<std::string, T*>(key, ptr));
    CGRAPH_FUNCTION_END
}


#endif // CGRAPH_GPARAMMANAGER_INL