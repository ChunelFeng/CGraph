/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamManager.h
@Time: 2021/6/11 6:59 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPARAMMANAGER_H
#define CGRAPH_GPARAMMANAGER_H

#include <unordered_map>
#include <string>
#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"
#include "GParam.h"

class GParamManager : public CObject {
public:
    template<typename T>
    CSTATUS create(const std::string& key) {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_WLOCK rLock(this->mtx_);
        if (params_map_.find(key) != params_map_.end()) {
            // 如果有，不重复创建
            return STATUS_OK;
        }

        T* ptr = new(std::nothrow) T();
        CGRAPH_ASSERT_NOT_NULL(ptr)

        params_map_.insert(std::pair<std::string, T*>(key, ptr));
        CGRAPH_FUNCTION_END
    }


    GParamPtr get(const std::string& key) {
        CGRAPH_WLOCK wLock(this->mtx_);
        auto result = params_map_.find(key);
        if (result == params_map_.end()) {
            return nullptr;
        }

        return params_map_.find(key)->second;;
    }


protected:
    CSTATUS run() override;


private:
    std::unordered_map<std::string, GParamPtr> params_map_;
    std::shared_mutex mtx_;
};


#endif //CGRAPH_GPARAMMANAGER_H
