/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GRegion.cpp
@Time: 2021/6/1 10:14 下午
@Desc: 
***************************/

#include "GRegion.h"
#include "../../_GOptimizer/GOptimizerInclude.h"
CGRAPH_NAMESPACE_BEGIN

GRegion::GRegion() : GGroup() {
    is_init_ = false;
    manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    element_type_ = GElementType::REGION;
    session_ = URandom<>::generateSession(CGRAPH_STR_REGION);
}


GRegion::~GRegion() {
    CGRAPH_DELETE_PTR(manager_)
}


CStatus GRegion::init() {
    CGRAPH_FUNCTION_BEGIN
    // 在这里将初始化所有的节点信息，并且实现分析，联通等功能
    CGRAPH_ASSERT_NOT_NULL(thread_pool_, manager_)

    // 在region中，需要专门的调度逻辑
    this->manager_->setThreadPool(thread_pool_);
    status = this->manager_->init();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GRegion::destroy() {
    CGRAPH_FUNCTION_BEGIN
    status = manager_->destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GRegion::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(manager_)

    status = manager_->run();
    CGRAPH_FUNCTION_END
}


CStatus GRegion::addElement(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element, manager_)

    manager_->manager_elements_.emplace(element);
    CGRAPH_FUNCTION_END
}


GRegionPtr GRegion::setGEngineType(GEngineType type) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(manager_)

    this->manager_->setEngineType(type);
    return this;
}


CVoid GRegion::dump(std::ostream& oss) {
    dumpElement(oss);
    dumpGroupLabelBegin(oss);
    oss << 'p' << this << "[shape=point height=0];\n";
    oss << "color=blue;\n";

    for (const auto& element : manager_->manager_elements_) {
        element->dump(oss);
    }

    dumpGroupLabelEnd(oss);

    for (const auto& element : run_before_) {
        dumpEdge(oss, this, element);
    }
}


CBool GRegion::isSerializable() const {
    if (nullptr == manager_) {
        return false;
    }

    return manager_->checkSerializable();
}


CStatus GRegion::addManagers(GParamManagerPtr paramManager,
                             GEventManagerPtr eventManager) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(paramManager, eventManager)

    this->setGParamManager(paramManager);
    this->setGEventManager(eventManager);
    for (auto* cur : manager_->manager_elements_) {
        CGRAPH_ASSERT_NOT_NULL(cur)
        status += cur->addManagers(paramManager, eventManager);
    }

    CGRAPH_FUNCTION_END
}


CBool GRegion::isSeparate(GElementCPtr a, GElementCPtr b) const {
    return GSeparateOptimizer::checkSeparate(manager_->manager_elements_, a, b);
}


CSize GRegion::trim() {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CSize result = 0;
    if (manager_) {
        result = GTrimOptimizer::trim(manager_->manager_elements_);
    }
    return result;
}

CGRAPH_NAMESPACE_END