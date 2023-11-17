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

    status += initEnv();
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
    status += repository_.reset();    // 等异步的逻辑执行，并且获取异步执行的结果信息
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

    status += repository_.destroy();
    status += schedule_.destroy();
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::process(CSize runTimes) {
    CGRAPH_FUNCTION_BEGIN
    status = init();
    CGRAPH_FUNCTION_CHECK_STATUS

    while (runTimes-- > 0
           && !status.isErr()
           && !repository_.isCancelState()) {
        /**
         * 1. 执行轮数（runTimes）没有结束
         * 2. 执行结果正常
         * 3. 没有进入取消状态
         */
        status += run();
    }

    status += destroy();
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::registerGGroup(GElementPPtr groupRef, const GElementPtrSet &dependElements,
                                  const std::string &name, CSize loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(*groupRef)

    auto group = dynamic_cast<GGroupPtr>(*groupRef);
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(nullptr == group, "input is not based on GGroup")
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(group->isRegistered(), "this group register duplicate")

    status = group->setElementInfo(dependElements, name, loop, this->param_manager_, this->event_manager_);
    CGRAPH_FUNCTION_CHECK_STATUS
    status = element_manager_->add(group);
    CGRAPH_FUNCTION_CHECK_STATUS
    repository_.insert(group);
    CGRAPH_FUNCTION_END
}


std::future<CStatus> GPipeline::asyncRun() {
    /**
     * 1. 确认当前pipeline已经初始化完毕
     * 2. 异步的执行 run() 方法，并且返回执行结果的 future 信息
     */
    CGRAPH_ASSERT_INIT_THROW_ERROR(true)

    return std::async(std::launch::async, [this] {
        return run();
    });
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
    oss << std::fixed << std::setprecision(2);    // 小数点最多展示2位数字

    oss << "\ndigraph CGraph {\n";
    oss << "compound=true;\n";

    for (const auto& element : element_manager_->manager_elements_) {
        CGRAPH_ASSERT_NOT_NULL(element)
        element->dump(oss);
    }

    oss << "}\n\n";
    std::cout.unsetf(std::ios::fixed);
    CGRAPH_FUNCTION_END
}


CStatus GPipeline::perf(std::ostream& oss) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    status = GPerf::perf(this);
    CGRAPH_FUNCTION_END
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
    CGRAPH_THROW_EXCEPTION_BY_STATUS(config.check())

    /**
     * 实际是将信息传递给 schedule中，如果是unique的话，就使用这个参数
     * 如果是 shared的话，其实配置是无效的
     */
    schedule_.config_ = config;
    return this;
}


GPipelinePtr GPipeline::setSharedThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_THROW_EXCEPTION_BY_STATUS(schedule_.makeType(ptr))
    return this;
}


GPipelinePtr GPipeline::setAutoCheck(CBool enable) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element_manager_)

    element_manager_->auto_check_enable_ = enable;
    return this;
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

    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((schedule_.type_ != GScheduleType::UNIQUE),    \
                                            "cannot set serial config without UNIQUE schedule")

    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!element_manager_->checkSerializable(),    \
                                       "cannot set serial config for this pipeline")

    UThreadPoolConfig config;
    config.default_thread_size_ = 0;    // 设置之后，不再开辟线程池，直接通过主线程执行pipeline的逻辑
    config.secondary_thread_size_ = 0;
    config.max_thread_size_ = 0;
    config.monitor_enable_ = false;
    config.batch_task_enable_ = false;
    schedule_.config_ = config;
    CGRAPH_FUNCTION_END
}


GPipelineState GPipeline::getCurState() const {
    return repository_.cur_state_;
}


CStatus GPipeline::initEnv() {
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

    status += repository_.init();
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
