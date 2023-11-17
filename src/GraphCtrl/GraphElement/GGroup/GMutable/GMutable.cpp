/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMutable.cpp
@Time: 2023/11/13 22:21
@Desc: 
***************************/

#include "GMutable.h"

CGRAPH_NAMESPACE_BEGIN

GMutable::GMutable() {
    element_type_ = GElementType::MUTABLE;
    session_ = URandom<>::generateSession(CGRAPH_STR_MUTABLE);
    manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
}


GMutable::~GMutable() {
    CGRAPH_DELETE_PTR(manager_)
}


CStatus GMutable::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)
    this->group_elements_arr_.emplace_back(element);
    CGRAPH_FUNCTION_END
}


CStatus GMutable::init() {
    CGRAPH_FUNCTION_BEGIN
    manager_->setThreadPool(thread_pool_);
    status = manager_->initEngine();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = GGroup::init();
    CGRAPH_FUNCTION_END
}


CStatus GMutable::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(manager_)

    /**
     * 1. 初始化内容
     * 2. 通过外部复写 reshape()，来实现关系设定。其中，通过 --> 设定的，是会自动恢复visible的
     * 3. 通过 manager 执行
     */
    setup();
    status = reshape(group_elements_arr_);
    CGRAPH_FUNCTION_CHECK_STATUS

    status = manager_->process({group_elements_arr_.begin(), group_elements_arr_.end()});
    CGRAPH_FUNCTION_END
}


CStatus GMutable::destroy() {
    CGRAPH_FUNCTION_BEGIN
    for (auto* element : group_elements_arr_) {
        // 链路中，可能会将部分内容设置为 visible 的信息。这里统一恢复一下
        element->setVisible(true);
    }

    status = GGroup::destroy();
    CGRAPH_FUNCTION_END
}


CVoid GMutable::setup() {
    for (auto* element : group_elements_arr_) {
        element->run_before_.clear();
        element->dependence_.clear();
        element->setLoop(1);
        element->setVisible(false);
    }
}

CGRAPH_NAMESPACE_END
