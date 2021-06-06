/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementManager.cpp
@Time: 2021/6/2 10:33 下午
@Desc: 
***************************/

#include "GElementManager.h"

GElementManager::GElementManager() : CObject() {

}

GElementManager::~GElementManager() = default;


GElementManager::GElementManager(const GElementManager& manager) {
    this->manager_elements_ = manager.manager_elements_;
}


GElementManager& GElementManager::operator=(const GElementManager& manager) {
    if (this == &manager) {
        return (*this);
    }

    this->manager_elements_ = manager.manager_elements_;
    return (*this);
}


CSTATUS GElementManager::init() {
    CGRAPH_FUNCTION_BEGIN
    status = preCheck();
    CGRAPH_FUNCTION_CHECK_STATUS

    // manager 中的所有element初始化
    status = std::all_of(manager_elements_.begin(), manager_elements_.end(),
                         [](GElementPtr element) {
        return STATUS_OK == element->init();
    }) ? STATUS_OK : STATUS_ERR;

    CGRAPH_FUNCTION_END
}


CSTATUS GElementManager::deinit() {
    CGRAPH_FUNCTION_BEGIN

    status = std::all_of(manager_elements_.begin(), manager_elements_.end(),
                         [](GElementPtr element) {
        return STATUS_OK == element->deinit();
    }) ? STATUS_OK : STATUS_ERR;

    CGRAPH_FUNCTION_END
}


/**
 * 无法执行manager的run方法
 * @return
 */
CSTATUS GElementManager::run() {
    CGRAPH_PROCESS_ERROR
}


CSTATUS GElementManager::preCheck() {
    CGRAPH_FUNCTION_BEGIN

    /**
     * 认定图可以连通的判定条件：
     * 1，当前节点仅有一个依赖
     * 2，当前节点依赖的节点，只有一个后继
     * 3，当前节点的依赖的后继，仍是当前节点
     */
    for (GElement* element : manager_elements_) {
        if (1 == element->dependence_.size()
            && 1 == (*element->dependence_.begin())->run_before_.size()
            && (*(element->dependence_.begin()))->run_before_.find(element) != (*(element->dependence_.begin()))->run_before_.end()) {
            element->linkable_ = true;
        }
    }

    CGRAPH_FUNCTION_END
}


CSTATUS GElementManager::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->manager_elements_.emplace(element);

    CGRAPH_FUNCTION_END
}


bool GElementManager::hasElement(GElementPtr element) const {
    if (nullptr == element) {
        return false;
    }

    return manager_elements_.find(element) != manager_elements_.end();
}


void GElementManager::deleteElement(GElementPtr element) {
    manager_elements_.erase(element);
}
