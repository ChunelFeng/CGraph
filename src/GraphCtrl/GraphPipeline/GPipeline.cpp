/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: Graphic.cpp
@Time: 2021/6/2 10:15 下午
@Desc: 
***************************/

#include "GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

GPipeline::GPipeline() {
    session_ = URandom<>::generateSession(CGRAPH_STR_PIPELINE);
    element_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GElementManager)
    param_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GParamManager)
    daemon_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GDaemonManager)
    event_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GEventManager)
}


GPipeline::~GPipeline() {
    // 结束的时候，会自动释放所有的element信息
    CGRAPH_DELETE_PTR(daemon_manager_)
    CGRAPH_DELETE_PTR(element_manager_)
    CGRAPH_DELETE_PTR(param_manager_)
    CGRAPH_DELETE_PTR(event_manager_)
}


CStatus GPipeline::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)    // 必须是非初始化的状态下，才可以初始化。反之同理
    CGRAPH_ASSERT_NOT_NULL(element_manager_, param_manager_, daemon_manager_, event_manager_)

    status += initSchedule();
    CGRAPH_FUNCTION_CHECK_STATUS

    status += param_manager_->init();
    status += event_manager_->init();
    status += element_manager_->init();
    status += daemon_manager_->init();    // daemon的初始化，需要晚于所有element的初始化
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(element_manager_, param_manager_)

    /**
     * 1. 将所有 GElement 的状态设定为 NORMAL
     * 2. 将所有的 GParam 设置为初始值
     * 3. 执行dag逻辑
     * 4. 将所有的 GParam 复原
     */
    status += repository_.setup();
    status += param_manager_->setup();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = element_manager_->run();
    param_manager_->resetWithStatus(status);
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::destroy() {
    CGRAPH_FUNCTION_BEGIN

    CGRAPH_ASSERT_INIT(true)
    CGRAPH_ASSERT_NOT_NULL(element_manager_, param_manager_, daemon_manager_, event_manager_)

    status += event_manager_->destroy();
    status += daemon_manager_->destroy();
    status += element_manager_->destroy();
    status += param_manager_->destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    status = schedule_.destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::process(CSize runTimes) {
    CGRAPH_FUNCTION_BEGIN
    status = init();
    CGRAPH_FUNCTION_CHECK_STATUS

    while (runTimes-- > 0) {
        status = run();
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    status = destroy();
    CGRAPH_FUNCTION_END
}


std::future<CStatus> GPipeline::asyncRun() {
    /**
     * 1. 确定是否已经初始化
     * 2. 确定线程资源是ok的（理论上，初始化后，就一定会有值）
     * 3. 异步的执行 run() 方法，并且返回执行结果的 future 信息
     */
    CGRAPH_ASSERT_INIT_THROW_ERROR(true)

    auto tp = schedule_.getThreadPool();
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(tp)

    return tp->commitWithPriority([this] {
        return run();
    }, CGRAPH_DEFAULT_PRIORITY);
}


std::future<CStatus> GPipeline::asyncProcess(CSize runTimes) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    return std::async(std::launch::async, [runTimes, this] {
        return process(runTimes);
    });
}


CStatus GPipeline::cancel() {
    CGRAPH_ASSERT_INIT(true)
    return repository_.pushAllState(GElementState::CANCEL);
}


CStatus GPipeline::yield() {
    CGRAPH_ASSERT_INIT(true)
    return repository_.pushAllState(GElementState::YIELD);
}


CStatus GPipeline::resume() {
    // 直接恢复正常状态好了
    CGRAPH_ASSERT_INIT(true)
    return repository_.pushAllState(GElementState::NORMAL);
}


CStatus GPipeline::dump(std::ostream& oss) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element_manager_)
    oss << "digraph CGraph {\n";
    oss << "compound=true;\n";

    for (const auto& element : element_manager_->manager_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        element->dump(oss);
    }

    oss << "}\n";
    CGRAPH_FUNCTION_END
}


GPipelinePtr GPipeline::setGElementRunTtl(CMSec ttl) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element_manager_, element_manager_->engine_)

    // 在element_manager中区执行信息了，所以ttl放到
    element_manager_->engine_->element_run_ttl_ = ttl;
    return this;
}


GPipelinePtr GPipeline::setGEngineType(GEngineType type) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element_manager_)

    element_manager_->setEngineType(type);
    return this;
}


GPipelinePtr GPipeline::setUniqueThreadPoolConfig(const UThreadPoolConfig& config) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    /**
     * 实际是将信息传递给 schedule中，如果是unique的话，就使用这个参数
     * 如果是 shared的话，其实配置是无效的
     */
    schedule_.config_ = config;
    CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL
}


GPipelinePtr GPipeline::setSharedThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    status = schedule_.makeType(ptr);
    CGRAPH_CHECK_STATUS_RETURN_THIS_OR_NULL
}


CStatus GPipeline::calcMaxPara(CSize& size) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element_manager_)

    status = element_manager_->calcMaxParaSize(size);
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::makeSerial() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(element_manager_)

    if (schedule_.type_ != GScheduleType::UNIQUE) {
        CGRAPH_RETURN_ERROR_STATUS("cannot set serial config without UNIQUE schedule")
    }

    if (!element_manager_->checkSerializable()) {
        CGRAPH_RETURN_ERROR_STATUS("cannot set serial config for this pipeline")
    }

    UThreadPoolConfig config;
    config.default_thread_size_ = 0;    // 设置之后，不再开辟线程池，直接通过主线程执行pipeline的逻辑
    config.secondary_thread_size_ = 0;
    config.max_thread_size_ = 0;
    config.monitor_enable_ = false;
    schedule_.config_ = config;
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::initSchedule() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(event_manager_, element_manager_)

    status = schedule_.init();
    CGRAPH_FUNCTION_CHECK_STATUS

    auto tp = schedule_.getThreadPool();
    event_manager_->setThreadPool(tp);
    // 这里设置线程池，是为了将tp信息，传递给内部的engine类
    element_manager_->setThreadPool(tp);

    // 设置所有的element 中的thread_pool
    repository_.setThreadPool(tp);

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
