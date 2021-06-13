
#ifndef CGRAPH_GPARAMMANAGER_INL
#define CGRAPH_GPARAMMANAGER_INL

template<typename T>
CSTATUS GParamManager::create(const std::string& key) {
    CGRAPH_FUNCTION_BEGIN
    if (params_map_.find(key) != params_map_.end()) {
        // 如果有，不重复创建
        return STATUS_OK;
    }

    CGRAPH_WRITE_LOCK wLock(this->lock_);
    T* ptr = new(std::nothrow) T();
    CGRAPH_ASSERT_NOT_NULL(ptr)

    params_map_.insert(std::pair<std::string, T*>(key, ptr));
    CGRAPH_FUNCTION_END
}


#endif // CGRAPH_GPARAMMANAGER_INL