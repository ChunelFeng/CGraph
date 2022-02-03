/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectObject.h
@Time: 2021/10/2 9:46 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTOBJECT_H
#define CGRAPH_GASPECTOBJECT_H

#include <string>

#include "GAspectDefine.h"
#include "../GraphObject.h"
#include "../GraphParam/GParamInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GAspectObject : public GraphObject {
public:
    ~GAspectObject() override {
        CGRAPH_DELETE_PTR(param_)
    }

    /**
     * 获取name信息
     * @return
     */
    virtual const std::string& getName() final {
        return name_;
    }

    /**
     * 获取切面参数内容
     * @return
     */
    template <typename T,
              std::enable_if_t<std::is_base_of<GAspectParam, T>::value, int> = 0>
    T* getAParam();

    /**
     * 设置切面参数内容
     * @param param
     */
    template <typename T,
              std::enable_if_t<std::is_base_of<GAspectParam, T>::value, int> = 0>
    GAspectObject* setAParam(T* param);

    /**
     * 获取pipeline中的参数信息
     * @tparam T
     * @param key
     * @return
     */
    template <typename T,
              std::enable_if_t<std::is_base_of<GParam, T>::value, int> = 0>
    T* getGParam(const std::string& key);

protected:
    /**
     * 设置名称
     * @param name
     */
    virtual GAspectObject* setName(const std::string& name) {
        this->name_ = name;
        return this;
    }

    /**
     * 设置pipeline中相关的参数信息
     * @param pm
     * @return
     */
    virtual GAspectObject* setPipelineParamManager(GParamManagerPtr pm) {
        pipeline_param_manager_ = pm;
        return this;
    }

    /**
     * GAspect 相关内容，不需要执行run方法
     * @return
     */
    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }

private:
    std::string name_;                                        // 切面类名称，跟 element 名称保持相同
    GAspectParamPtr param_ { nullptr };                       // 参数信息
    GParamManagerPtr pipeline_param_manager_ { nullptr };     // 对应 pipeline 中参数管理器

    friend class GAspectManager;
    friend class GElement;
};

using GAspectObjectPtr = GAspectObject *;

CGRAPH_NAMESPACE_END

#include "GAspectObject.inl"

#endif //CGRAPH_GASPECTOBJECT_H
