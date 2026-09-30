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
     * @return
     */                                                                                 \
    template<typename TGParam,                                                          \
             c_enable_if_t<std::is_base_of<GParam, TGParam>::value, int> = 0>           \
    CStatus createGParam(const std::string& key) {                                      \
        CGRAPH_ASSERT_NOT_NULL(param_manager_)                                          \
        return param_manager_->create<TGParam>(key);                                    \
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
        return param_manager_->get<TGParam>(key);                                       \
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
     * @return
     */                                                                                 \
    std::vector<std::string> getGParamKeys() {                                          \
        CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param_manager_)                              \
        return param_manager_->getKeys();                                               \
    }                                                                                   \
                                                                                        \
CGRAPH_INTERNAL_ACCESS:                                                                 \
    GParamPtr __getGParam_4expose(const std::string& key) {                             \
        return param_manager_ ? param_manager_->__get_4expose(key) : nullptr;           \
    }                                                                                   \
                                                                                        \
    GParamPtr __getGParamWithNoEmpty_4expose(const std::string& key) {                  \
        CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(param_manager_);                             \
        auto param = param_manager_->__get_4expose(key);                                \
        CGRAPH_THROW_EXCEPTION_BY_CONDITION(!param, "param [" + key + "] is null");     \
        return param;                                                                   \
    }                                                                                   \
                                                                                        \
    CStatus __createGParam_4expose(GParamPtr param, const std::string& key) {           \
        CGRAPH_ASSERT_NOT_NULL(param_manager_)                                          \
        return param_manager_->__create_4expose(param, key);                            \
    }                                                                                   \
                                                                                        \
    CStatus __removeGParam_4expose(const std::string& key) {                            \
        CGRAPH_ASSERT_NOT_NULL(param_manager_)                                          \
        return param_manager_->__remove_4expose(key);                                   \
    }                                                                                   \
                                                                                        \
    CBool __hasGParam_4expose(const std::string& key) {                                 \
        return param_manager_ ? param_manager_->__has_4expose(key) : false;             \
    }                                                                                   \
                                                                                        \
private:                                                                                \
    /**
     * 设置统一管控参数类
     * @param pm
     * @return
     */                                                                                 \
    virtual void* setGParamManager(GParamManagerPtr pm) {                               \
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
