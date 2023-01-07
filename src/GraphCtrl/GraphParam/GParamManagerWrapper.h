/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamManagerWrap.h
@Time: 2022/12/18 18:39
@Desc: 本类的子类型，有获取GParam的功能
***************************/

#ifndef CGRAPH_GPARAMMANAGERWRAPPER_H
#define CGRAPH_GPARAMMANAGERWRAPPER_H

#include <string>

#include "GParam.h"
#include "GParamManager.h"

CGRAPH_NAMESPACE_BEGIN

class GParamManagerWrapper : public GParamObject {
protected:
    /**
     * 设置统一的参数管理类
     * @param pm
     * @return
     */
    GParamManagerWrapper* setGParamManager(const GParamManagerPtr pm) {
        param_manager_ = pm;
        return this;
    }

    /**
     * 创建参数信息
     * @tparam T
     * @param key
     * @return
     */
    template<typename T,
            std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    CStatus createGParam(const std::string& key) {
        CGRAPH_ASSERT_NOT_NULL(param_manager_)
        return param_manager_->create<T>(key);
    }

    /**
     * 获取参数信息
     * @tparam T
     * @param key
     * @return
     */
    template<typename T,
            std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    T* getGParam(const std::string& key) {
        CGRAPH_ASSERT_NOT_NULL_RETURN_NULL(param_manager_)
        return param_manager_->get<T>(key);
    }

    /**
     * 获取参数信息，未获得的情况下，抛出异常
     * @tparam T
     * @param key
     * @return
     */
    template<typename T,
            std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    T* getGParamWithNoEmpty(const std::string& key) {
        auto* param = param_manager_->get<T>(key);
        if (nullptr == param) {
            CGRAPH_THROW_EXCEPTION("param [" + key + "] is null")
        }
        return param;
    }


private:
    GParamManagerPtr param_manager_ = nullptr;    // GParam管理类，必须在pipeline一层创建和销毁。

    friend class GElement;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAMMANAGERWRAPPER_H
