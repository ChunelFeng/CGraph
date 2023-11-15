/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementManager.cpp
@Time: 2021/6/2 10:33 下午
@Desc: 
***************************/

#include "GElementManager.h"
#include "_GOptimizer/GOptimizerInclude.h"

CGRAPH_NAMESPACE_BEGIN

GElementManager::GElementManager() {
    engine_ = nullptr;
}


GElementManager::~GElementManager() {
    /**
     * manager中的节点，在析构的时候不需要释放。
     * 所有的节点信息在GPipeLine类中统一申请和释放
     */
    CGRAPH_DELETE_PTR(engine_)
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
        element->trigger_times_ = 0;
    }

    CGRAPH_FUNCTION_END
}


CStatus GElementManager::destroy() {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr element : manager_elements_) {
        status = element->fatProcessor(CFunctionType::DESTROY);
        CGRAPH_FUNCTION_CHECK_STATUS
        element->is_init_ = false;
        element->trigger_times_ = 0;
    }

    CGRAPH_DELETE_PTR(engine_)
    CGRAPH_FUNCTION_END
}


CStatus GElementManager::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(engine_)

    status = engine_->run();    // 通过引擎来执行全部的逻辑
    CGRAPH_FUNCTION_CHECK_STATUS

    if (auto_check_enable_) {
        // 默认是需要check一下执行结果的。如果为了增加一点效率，也可以通过外部设置不检查
        status = engine_->afterRunCheck();
    }
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
    for (auto* element : manager_elements_) {
        CGRAPH_DELETE_PTR(element)
    }
    manager_elements_.clear();
    CGRAPH_FUNCTION_END
}


GElementManagerPtr GElementManager::setScheduleStrategy(int strategy) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(engine_)
    /**
     * 如果是 region中的 GElementManager，需要在init完成后，进行这一步赋值
     * 否则会因为 engine_ 为空，而导致崩溃
     */
    engine_->schedule_strategy_ = strategy;
    return this;
}


GElementManagerPtr GElementManager::setEngineType(GEngineType engineType) {
    engine_type_ = engineType;
    return this;
}


CStatus GElementManager::initEngine() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_DELETE_PTR(engine_)

    switch (engine_type_) {
        case GEngineType::STATIC : engine_ = CGRAPH_SAFE_MALLOC_COBJECT(GStaticEngine) break;
        case GEngineType::DYNAMIC : engine_ = CGRAPH_SAFE_MALLOC_COBJECT(GDynamicEngine) break;
        default: CGRAPH_RETURN_ERROR_STATUS("unknown engine type")
    }

    engine_->thread_pool_ = thread_pool_;
    status = engine_->setup(manager_elements_);
    CGRAPH_FUNCTION_END
}


GElementManagerPtr GElementManager::setThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(ptr)
    this->thread_pool_ = ptr;
    return this;
}


CStatus GElementManager::calcMaxParaSize(CSize& size) {
    CGRAPH_FUNCTION_BEGIN
    GMaxParaOptimizer op;
    if (op.match(manager_elements_)) {
        size = op.getMaxParaSize(manager_elements_);
    } else {
        // 遇到不可以解析的情况，将size赋值为0，告知上游
        size = 0;
        status = CErrStatus("cannot calculate max parallel size within groups");
    }
    CGRAPH_FUNCTION_END
}


CBool GElementManager::checkSerializable() {
    if (engine_type_ != GEngineType::DYNAMIC) {
        return false;    // 目前仅支持动态引擎的执行方式
    }

    /**
     * 判定思路：
     * 1. 内部的element，均为可串行执行的
     * 2. 当前element，不超过1个前驱或者后继
     * 3. 有且仅有一个起点，一个终点
     * 4. 有超时逻辑
     */
    int frontSize = 0, tailSize = 0;
    for (auto& cur : manager_elements_) {
        if (!cur->isSerializable()
            || cur->run_before_.size() > 1
            || cur->dependence_.size() > 1
            || cur->isAsync()) {
            return false;
        }

        if (cur->dependence_.empty()) {
            frontSize++;
        }
        if (cur->run_before_.empty()) {
            tailSize++;
        }
    }

    return (1 == frontSize) && (1 == tailSize);
}


CStatus GElementManager::process(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(engine_)

    // 主要是给 mutable 使用
    status += engine_->setup(elements);
    status += run();
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
