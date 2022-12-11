/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementManager.cpp
@Time: 2021/6/2 10:33 下午
@Desc: 
***************************/

#include "GElementManager.h"

CGRAPH_NAMESPACE_BEGIN

GElementManager::GElementManager() {
    engine_ = nullptr;
}

GElementManager::~GElementManager() {
    /**
     * manager中的节点，在析构的时候不需要释放。
     * 所有的节点信息在GPipeLine类中统一申请和释放
     */
    CGRAPH_DELETE_PTR(engine_);
}


CStatus GElementManager::init() {
    CGRAPH_FUNCTION_BEGIN

    /** 首先判定，注册的element全部不为空 */
    for (auto element : manager_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
    }

    status = initEngine();
    CGRAPH_FUNCTION_CHECK_STATUS

    for (GElementPtr element : manager_elements_) {
        status = element->fatProcessor(CFunctionType::INIT);
        CGRAPH_FUNCTION_CHECK_STATUS
        element->is_init_ = true;
    }

    CGRAPH_FUNCTION_END
}


CStatus GElementManager::destroy() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : manager_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)

        status = element->fatProcessor(CFunctionType::DESTROY);
        CGRAPH_FUNCTION_CHECK_STATUS
        element->is_init_ = false;
    }

    CGRAPH_DELETE_PTR(engine_)
    CGRAPH_FUNCTION_END
}


CStatus GElementManager::run() {
    CGRAPH_FUNCTION_BEGIN

    status = engine_->run();    // 通过引擎来执行全部的逻辑
    CGRAPH_FUNCTION_END
}


CStatus GElementManager::add(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    this->manager_elements_.emplace(element);

    CGRAPH_FUNCTION_END
}


CBool GElementManager::find(GElementPtr element) const {
    if (nullptr == element) {
        return false;
    }

    return manager_elements_.end() != manager_elements_.find(element);
}


CStatus GElementManager::remove(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    manager_elements_.erase(element);
    CGRAPH_FUNCTION_END
}


CStatus GElementManager::clear() {
    CGRAPH_FUNCTION_BEGIN

    for (auto element : manager_elements_) {
        CGRAPH_DELETE_PTR(element)
    }

    manager_elements_.clear();
    CGRAPH_FUNCTION_END
}


GElementManager* GElementManager::setScheduleStrategy(int strategy) {
    if (nullptr == engine_) {
        return nullptr;
    }

    engine_->schedule_strategy_ = strategy;
    return this;
}


CStatus GElementManager::initEngine() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_DELETE_PTR(engine_)
    // 这里可以根据一系列策略，来确定执行引擎的类型。暂时默认为是静态执行的
    engine_ = CGRAPH_SAFE_MALLOC_COBJECT(GStaticEngine)
    status = engine_->setUp(manager_elements_);

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
