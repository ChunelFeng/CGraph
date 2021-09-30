/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectObject.h
@Time: 2021/9/29 8:59 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTOBJECT_H
#define CGRAPH_GASPECTOBJECT_H

#include "../GElement.h"

template <typename T>
class GAspectObject : public GElement {
protected:
    /**
     * aspect类构造函数
     * 初始化内部elemet信息
     */
    explicit GAspectObject() {
        element_obj_ = CGRAPH_SAFE_MALLOC_COBJECT(T);
    }

    /**
     * aspect类析构函数
     * 析构element信息
     */
    ~GAspectObject() override {
        CGRAPH_DELETE_PTR(element_obj_)
    }

    /**
     * init()函数切面开始期间动作
     */
    virtual void beginInit() = 0;

    /**
     * init()函数切面结束期间动作
     */
    virtual void finishInit(CSTATUS status) = 0;

    /**
     * run()函数切面开始期间动作
     */
    virtual void beginRun() = 0;

    /**
     * run()函数切面结束期间动作
     */
    virtual void finishRun(CSTATUS status) = 0;

    /**
     * deinit()函数切面开始期间动作
     */
    virtual void beginDeinit() = 0;

    /**
     * init()函数切面结束期间动作
     */
    virtual void finishDeinit(CSTATUS status) = 0;

    /**
     * 初始化函数
     * @return
     */
    CSTATUS init() final {
        CGRAPH_FUNCTION_BEGIN

        CGRAPH_ASSERT_NOT_NULL(element_obj_)
        /* 对内部值name等信息，进行赋值 */
        auto ptr = dynamic_cast<GElementPtr>(element_obj_);
        ptr->name_ = this->name_;
        ptr->param_manager_ = this->param_manager_;

        beginInit();
        status = ptr->init();
        finishInit(status);

        CGRAPH_FUNCTION_END
    }

    /**
     * 执行函数
     * @return
     */
    CSTATUS run() final {
        CGRAPH_FUNCTION_BEGIN

        CGRAPH_ASSERT_NOT_NULL(GAspectObject<T>::element_obj_);
        beginRun();
        status = dynamic_cast<GElementPtr>(GAspectObject<T>::element_obj_)->run();
        finishRun(status);

        CGRAPH_FUNCTION_END
    }

    /**
     * 逆初始化函数
     * @return
     */
    CSTATUS deinit() final {
        CGRAPH_FUNCTION_BEGIN
        CGRAPH_ASSERT_NOT_NULL(element_obj_)
        auto ptr = dynamic_cast<GElementPtr>(element_obj_);

        beginDeinit();
        status = ptr->deinit();
        finishDeinit(status);

        CGRAPH_FUNCTION_END
    }


protected:
    T* element_obj_ { nullptr };               // 被执行的 element 信息
};


#endif //CGRAPH_GASPECTOBJECT_H
