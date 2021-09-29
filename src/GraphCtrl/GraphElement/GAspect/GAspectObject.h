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
     * 初始化函数
     * @return
     */
    CSTATUS init() final {
        CGRAPH_ASSERT_NOT_NULL(element_obj_)
        auto ptr = dynamic_cast<GElementPtr>(element_obj_);
        ptr->name_ = this->name_;
        ptr->session_ = this->session_;
        return ptr->init();
    }


    /**
     * 逆初始化函数
     * @return
     */
    CSTATUS deinit() final {
        CGRAPH_ASSERT_NOT_NULL(element_obj_)
        auto ptr = dynamic_cast<GElementPtr>(element_obj_);
        return ptr->deinit();
    }


protected:
    T* element_obj_ { nullptr };               // 被执行的 element 信息
};


#endif //CGRAPH_GASPECTOBJECT_H
