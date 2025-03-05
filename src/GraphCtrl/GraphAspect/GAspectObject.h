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
#include "../GraphEvent/GEventInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GElement;

class GAspectObject : public GraphObject,
                      public CDescInfo {
protected:
    explicit GAspectObject();

    ~GAspectObject() override;

    /**
     * 获取切面参数内容
     * @return
     */
    template <typename T,
              c_enable_if_t<std::is_base_of<GAspectParam, T>::value, int> = 0>
    T* getAParam();

    /**
     * 设置切面参数内容
     * @param param
     */
    template <typename T,
              c_enable_if_t<std::is_base_of<GAspectParam, T>::value, int> = 0>
    GAspectObject* setAParam(T* param);

    const std::string& getName() const override;

    CGRAPH_NO_ALLOWED_COPY(GAspectObject)

    CGRAPH_DECLARE_GPARAM_MANAGER_WRAPPER

    CGRAPH_DECLARE_GEVENT_MANAGER_WRAPPER

private:
    /**
     * GAspect 相关内容，不需要执行run方法
     * @return
     */
    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }

private:
    GAspectParamPtr param_ { nullptr };                       // 参数信息
    GParamManagerPtr param_manager_ { nullptr };              // GParam参数管理类
    GEventManagerPtr event_manager_ { nullptr };              // 事件管理类
    GElement* belong_ { nullptr };                            // 从属的 element信息

    friend class GAspectManager;
    friend class GAspect;
    friend class GElement;

public:
    const std::string& __getName_4py() const {
        return getName();
    }
};

using GAspectObjectPtr = GAspectObject *;

CGRAPH_NAMESPACE_END

#include "GAspectObject.inl"

#endif //CGRAPH_GASPECTOBJECT_H
