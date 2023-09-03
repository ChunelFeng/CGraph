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
#include <vector>

#include "GParam.h"
#include "GParamManager.h"

CGRAPH_NAMESPACE_BEGIN

#define CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER                                           \
    /**
     * 创建param信息，如果创建成功，则直接返回ok
     * @tparam TGParam
     * @param key
     * @param backtrace
     * @return
     */                                                                                 \
    template<typename TGParam,                                                          \
             c_enable_if_t<std::is_base_of<GParam, TGParam>::value, int> = 0>           \
    CStatus createGParam(const std::string& key, CBool backtrace = false) {             \
        CGRAPH_ASSERT_NOT_NULL(param_manager_)                                          \
        return param_manager_->create<TGParam>(key, backtrace);                         \
    }                                                                                   \
                                                                                        \
    /**
     * 获取参数信息，如果未找到，则返回nullptr
     * @tparam TGParam
     * @param key
     * @return
     */                                                                                 \
    template<typename TGParam,                                                          \
             c_enable_if_t<std::is_base_of<GParam, TGParam>::value, int> = 0>           \
    TGParam* getGParam(const std::string& key) {                                        \
        CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param_manager_)                              \
        auto param = param_manager_->get<TGParam>(key);                                 \
        if (nullptr != param) {                                                         \
            param->addBacktrace(name_.empty() ? session_ : name_);                      \
        }                                                                               \
        return param;                                                                   \
    }                                                                                   \
                                                                                        \
    /**
     * 获取参数信息，如果未找到，则返回nullptr
     * @tparam TGParam
     * @param key
     * @return
     */                                                                                 \
    template<typename TGParam,                                                          \
             c_enable_if_t<std::is_base_of<GParam, TGParam>::value, int> = 0>           \
    TGParam* getGParamWithNoEmpty(const std::string& key) {                             \
        auto* param = getGParam<TGParam>(key);                                          \
        if (nullptr == param) {                                                         \
            CGRAPH_THROW_EXCEPTION("param [" + key + "] is null")                       \
        }                                                                               \
        return param;                                                                   \
    }                                                                                   \
                                                                                        \
    /**
     * 删除param信息
     * @param key
     * @return
     */                                                                                 \
    CStatus removeGParam(const std::string& key) {                                      \
        CGRAPH_ASSERT_NOT_NULL(param_manager_)                                          \
        return param_manager_->removeByKey(key);                                        \
    }                                                                                   \
                                                                                        \
    /**
     * 获取所有的keys信息
     * @param keys
     * @return
     */                                                                                 \
    CStatus getGParamKeys(std::vector<std::string>& keys) {                             \
        CGRAPH_ASSERT_NOT_NULL(param_manager_)                                          \
        return param_manager_->getKeys(keys);                                           \
    }                                                                                   \
                                                                                        \
private:                                                                                \
    /**
     * 设置统一管控参数类
     * @param pm
     * @return
     */                                                                                 \
    void* setGParamManager(GParamManagerPtr pm) {                                       \
        this->param_manager_ = pm;                                                      \
        return this;                                                                    \
    }                                                                                   \
                                                                                        \
protected:                                                                              \
                                                                                        \


#define CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER_WITH_MEMBER                               \
private:                                                                                \
    GParamManagerPtr param_manager_ = nullptr;                                          \
protected:                                                                              \
    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER                                               \
                                                                                        \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAMMANAGERWRAPPER_H
