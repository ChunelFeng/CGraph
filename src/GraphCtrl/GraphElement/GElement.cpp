/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElement.cpp
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#include <algorithm>

#include "GElement.h"
#include "../GraphPipeline/_GPerf/GPerfInclude.h"

CGRAPH_NAMESPACE_BEGIN

GElement::~GElement() {
    CGRAPH_DELETE_PTR(perf_info_)
    CGRAPH_DELETE_PTR(aspect_manager_)
    for (auto& param : local_params_) {
        CGRAPH_DELETE_PTR(param.second)    // 依次删除本地的参数信息
    }
}


CVoid GElement::beforeRun() {
    this->done_ = false;
    this->left_depend_.store(dependence_.size(), std::memory_order_release);
}


    #ifdef _WIN32
CVoidPtr GElement::setName(const std::string& name) {
    #else
GElementPtr GElement::setName(const std::string& name) {
    #endif
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    this->name_ = name.empty() ? this->session_ : name;
    return this;
}


GElementPtr GElement::setLoop(CSize loop) {
    // 由于运行机制问题，loop执行的element，不支持异步执行
    CGRAPH_ASSERT_MUTABLE_INIT_THROW_ERROR(false)
    CGRAPH_THROW_EXCEPTION_BY_CONDITION((timeout_ > CGRAPH_DEFAULT_ELEMENT_TIMEOUT && loop != CGRAPH_DEFAULT_LOOP_TIMES),     \
                                        "cannot set loop value when timeout is bigger than 0")

    this->loop_ = loop;
    return this;
}


GElementPtr GElement::setLevel(CLevel level) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    this->level_ = level;
    return this;
}


GElementPtr GElement::setVisible(CBool visible) {
    CGRAPH_ASSERT_MUTABLE_INIT_THROW_ERROR(false)

    this->visible_ = visible;
    return this;
}


GElementPtr GElement::setBindingIndex(CIndex index) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    /**
     * 由于内部有调度机制，不保证绑定线程后，一定在固定线程上执行。
     * 仅保证优先考虑使用绑定线程执行
     */
    this->binding_index_ = index;
    return this;
}


GElementPtr GElement::setTimeout(CMSec timeout, GElementTimeoutStrategy strategy) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    CGRAPH_THROW_EXCEPTION_BY_CONDITION((timeout < CGRAPH_DEFAULT_ELEMENT_TIMEOUT),     \
                                   "timeout value cannot smaller than 0")
    CGRAPH_THROW_EXCEPTION_BY_CONDITION((loop_ > CGRAPH_DEFAULT_LOOP_TIMES && CGRAPH_DEFAULT_ELEMENT_TIMEOUT != timeout),     \
                                        "cannot set timeout value when loop bigger than 1")

    this->timeout_ = timeout;
    this->timeout_strategy_ = strategy;
    return this;
}


GElementPtr GElement::setMacro(CBool macro) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)

    // 目前仅针对非group逻辑生效
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(isGGroup(), "cannot set group as macro")
    is_marco_ = macro;
    return this;
}


GElementRef GElement::operator--(int) noexcept {
    try {
        this->setVisible(true);
    } catch (const CException&) {
        CGRAPH_ECHO("[warning] default set visible failed.");
    }

    return (*this);
}


GElementRef GElement::operator>(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(element)
    CGRAPH_ASSERT_MUTABLE_INIT_THROW_ERROR(false)
    CGRAPH_THROW_EXCEPTION_BY_STATUS(element->addDependGElements({this}))

    // 默认通过 这种方式注入的内容，都设置成 visible 的状态
    this->setVisible(true);
    element->setVisible(true);
    return (*this);
}


GElementRef GElement::operator&(GElementPtr element) {
    return operator>(element);
}


GElement& GElement::operator*(CSize loop) noexcept {
    try {
        this->setLoop(loop);
    } catch (const CException&) {
        CGRAPH_ECHO("[warning] default set loop failed.");
    }

    return (*this);
}


CBool GElement::isAsync() const {
    // 如果timeout != 0, 则异步执行
    return this->timeout_ != CGRAPH_DEFAULT_ELEMENT_TIMEOUT;
}


CBool GElement::isRegistered() const {
    return (nullptr != param_manager_) && (nullptr != event_manager_);
}


CStatus GElement::addDependGElements(const GElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    if (!isMutable()) {
        // 如果是 mutable的逻辑，则可以在 init之后，修改依赖关系
        CGRAPH_ASSERT_INIT(false)
    }

    for (GElementPtr element: elements) {
        CGRAPH_ASSERT_NOT_NULL(element)
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((element->belong_ != this->belong_),     \
        element->getName() + " cannot depend because not same belong info")
        if (this == element) {
            continue;
        }

        element->run_before_.insert(this);
        this->dependence_.insert(element);
    }

    this->left_depend_.store(this->dependence_.size(), std::memory_order_release);
    CGRAPH_FUNCTION_END
}


CStatus GElement::addElementInfo(const GElementPtrSet& depends,
                                 const std::string& name, CSize loop) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    // 添加依赖的时候，可能会出现异常情况。故在这里提前添加 && 做判定
    status = this->addDependGElements(depends);
    CGRAPH_FUNCTION_CHECK_STATUS

    this->setLoop(loop);
    this->setName(name);
    CGRAPH_FUNCTION_END
}


CStatus GElement::addManagers(GParamManagerPtr paramManager, GEventManagerPtr eventManager) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(paramManager, eventManager)

    this->setGParamManager(paramManager);
    this->setGEventManager(eventManager);
    if (aspect_manager_) {
        aspect_manager_->setGParamManager(paramManager);
        aspect_manager_->setGEventManager(eventManager);
    }

    CGRAPH_FUNCTION_END
}


CStatus GElement::doAspect(const internal::GAspectType& aspectType, const CStatus& curStatus) {
    CGRAPH_FUNCTION_BEGIN

    // 如果切面管理类为空，或者未添加切面，直接返回
    if (this->aspect_manager_
        && 0 != this->aspect_manager_->getSize()) {
        status = aspect_manager_->reflect(aspectType, curStatus);
    }

    CGRAPH_FUNCTION_END
}


CStatus GElement::fatProcessor(const CFunctionType& type) {
    CGRAPH_FUNCTION_BEGIN

    if (unlikely(!visible_)) {
        /**
         * 如果当前的 element 因为被remove等原因，变成 不可见的状态
         * 则不运行。但不是实际删除当前节点信息
         */
        CGRAPH_FUNCTION_END
    }

    try {
        switch (type) {
            case CFunctionType::RUN: {
                if (!is_prepared_) {
                    /** 第一次执行的时候，预先执行一下 prepareRun方法 */
                    status = prepareRun();
                    CGRAPH_FUNCTION_CHECK_STATUS
                    is_prepared_ = true;
                }

                for (CSize i = 0; i < this->loop_ && status.isOK() && GElementState::NORMAL == this->getCurState(); i++) {
                    /** 执行带切面的run方法 */
                    status += doAspect(internal::GAspectType::BEGIN_RUN);
                    CGRAPH_FUNCTION_CHECK_STATUS
                    do {
                        status += isAsync() ? asyncRun() : run();
                        /**
                         * 在实际run结束之后，首先需要判断一下是否进入yield状态了。
                         * 接下来，如果状态是ok的，并且被条件hold住，则循环执行
                         * 默认所有element的isHold条件均为false，即不hold，即执行一次
                         * 可以根据需求，对任意element类型，添加特定的isHold条件
                         * */
                    } while (checkYield(), this->isHold() && status.isOK());
                    doAspect(internal::GAspectType::FINISH_RUN, status);
                }

                CGRAPH_THROW_EXCEPTION_BY_STATUS(checkRunResult())
                break;
            }
            case CFunctionType::INIT: {
                concerned_params_.clear();    // 仅需要记录这一轮使用到的 GParam 信息
                is_prepared_ = false;
                status = doAspect(internal::GAspectType::BEGIN_INIT);
                CGRAPH_FUNCTION_CHECK_STATUS
                status = init();
                doAspect(internal::GAspectType::FINISH_INIT, status);
                break;
            }
            case CFunctionType::DESTROY: {
                status = doAspect(internal::GAspectType::BEGIN_DESTROY);
                CGRAPH_FUNCTION_CHECK_STATUS
                status = destroy();
                doAspect(internal::GAspectType::FINISH_DESTROY, status);
                break;
            }
            default:
                CGRAPH_RETURN_ERROR_STATUS("get function type error")
        }
    } catch (const CException& ex) {
        doAspect(internal::GAspectType::ENTER_CRASHED);
        status = crashed(ex);
    }

    CGRAPH_FUNCTION_END
}


CStatus GElement::prepareRun() {
    CGRAPH_EMPTY_FUNCTION
}


CStatus GElement::checkRunResult() {
    CGRAPH_EMPTY_FUNCTION
}


CBool GElement::isHold() {
    /**
     * 默认仅返回false
     * 可以根据自己逻辑，来实现"持续循环执行，直到特定条件出现的时候停止"的逻辑
     */
    return false;
}


CBool GElement::isMatch() {
    /**
     * 默认仅返回false
     * 主要面对写入 MultiCondition 的时候，做判断当前element是否被执行
     */
    return false;
}


CBool GElement::isTimeout() const {
    /**
     * 判断的标准是：
     * 1. 如果当前节点超时，则认定为超时
     * 2. 如果当前节点所在的group超时，则也认定为超时
     */
    CBool result = (GElementState::TIMEOUT == cur_state_.load(std::memory_order_acquire));
    GElementPtr belong = this->belong_;
    while (!result && belong) {
        result = (GElementState::TIMEOUT == belong->cur_state_.load(std::memory_order_acquire));
        belong = belong->belong_;
    }

    return result;
}


CBool GElement::isMutable() const {
    // 写入 GMutable的 element，属于 mutable，可以在运行时，修改依赖关系
    return belong_ && GElementType::MUTABLE == belong_->element_type_;
}


CBool GElement::isMacro() const {
    return is_marco_;
}


CStatus GElement::crashed(const CException& ex) {
    (void)(this);
    return CStatus(internal::STATUS_CRASH, ex.what());
}


CIndex GElement::getThreadIndex() {
    CGRAPH_THROW_EXCEPTION_BY_CONDITION((nullptr == thread_pool_),    \
        this->getName() + " getThreadIndex with no thread pool")    // 理论不可能出现的情况

    auto tid = (CSize)std::hash<std::thread::id>{}(std::this_thread::get_id());
    return thread_pool_->getThreadIndex(tid);
}


GElementPtr GElement::setThreadPool(UThreadPoolPtr ptr) {
    CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(ptr)
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    this->thread_pool_ = ptr;
    return this;
}


CVoid GElement::dump(std::ostream& oss) {
    dumpElement(oss);

    for (const auto& node : run_before_) {
        dumpEdge(oss, this, node);
    }
}


CVoid GElement::dumpEdge(std::ostream& oss, GElementPtr src, GElementPtr dst, const std::string& label) {
    (void)(this);
    if (src->isGGroup() && dst->isGGroup()) {
        // 在group的逻辑中，添加 cluster_ 的信息
        oss << 'p' << src << " -> p" << dst << label << "[ltail=cluster_p" << src << " lhead=cluster_p" << dst << "]";
    } else if (src->isGGroup() && !dst->isGGroup()) {
        oss << 'p' << src << " -> p" << dst << label << "[ltail=cluster_p" << src << "]";
    } else if (!src->isGGroup() && dst->isGGroup()) {
        oss << 'p' << src << " -> p" << dst << label << "[lhead=cluster_p" << dst << "]";
    } else {
        oss << 'p' << src << " -> p" << dst << label;
    }

    if (src->perf_info_ && src->perf_info_->in_longest_path_
        && dst->perf_info_ && dst->perf_info_->in_longest_path_) {
        oss << "[color=red]";
    }
    oss << ";\n";
}


CVoid GElement::dumpElement(std::ostream& oss) {
    dumpElementHeader(oss);
    dumpPerfInfo(oss);

    oss << "\"];\n";
    if (this->loop_ > 1 && !this->isGGroup()) {
        oss << 'p' << this << " -> p" << this << "[label=\"" << this->loop_ << "\"]" << ";\n";
    }
}


CVoid GElement::dumpElementHeader(std::ostream& oss) {
    oss << 'p' << this << "[label=\"";
    if (this->name_.empty()) {
        oss << 'p' << this;    // 如果没有名字，则通过当前指针位置来代替
    } else {
        oss << this->name_;
    }
}


CVoid GElement::dumpPerfInfo(std::ostream& oss) {
    if (perf_info_ && perf_info_->loop_ > 0) {
        // 包含 perf信息的情况
        oss << "\n";
        oss << "[start " << perf_info_->first_start_ts_;
        oss << "ms, finish " << perf_info_->last_finish_ts_ << "ms,\n";
        oss << "per_cost " << (perf_info_->accu_cost_ts_ / perf_info_->loop_);
        if (perf_info_->loop_ > 1) {
            oss << "ms, total_cost " << perf_info_->accu_cost_ts_;
        }
        oss << "ms]";
    }
}


CVoid GElement::checkYield() {
    std::unique_lock<std::mutex> lk(yield_mutex_);
    this->yield_cv_.wait(lk, [this] {
        return GElementState::YIELD != cur_state_.load(std::memory_order_acquire);
    });
}


CBool GElement::isGGroup() const {
    // 按位与 GROUP有值，表示是 GROUP的逻辑
    return (long(element_type_) & long(GElementType::GROUP)) > 0;
}


CBool GElement::isGAdaptor() const {
    return (long(element_type_) & long(GElementType::ADAPTER)) > 0;
}


CBool GElement::isGNode() const {
    return GElementType::NODE == element_type_;
}


GElementState GElement::getCurState() const {
    /**
     * 如果有超时逻辑的话，优先判断
     * 否则就是当前的状态
     */
    auto state = this->isTimeout() ? GElementState::TIMEOUT : cur_state_.load(std::memory_order_acquire);
    return state;
}


CIndex GElement::getBindingIndex() const {
    return this->binding_index_;
}


GElementRelation GElement::getRelation() const {
    GElementRelation relation;
    relation.predecessors_ = this->dependence_.asVector();    // 前驱
    relation.successors_ = this->run_before_.asVector();    // 后继
    relation.belong_ = this->belong_;    // 从属信息

    return relation;
}


CStatus GElement::removeDepend(GElementPtr element) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!dependence_.hasValue(element),
                                            element->getName() + " is not in [" + getName() + "]'s depends.")

    dependence_.remove(element);
    element->run_before_.remove(this);
    left_depend_.store(dependence_.size(), std::memory_order_release);
    CGRAPH_FUNCTION_END
}


CBool GElement::isSerializable() const {
    return true;
}


CStatus GElement::popLastAspect() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    CGRAPH_ASSERT_NOT_NULL(aspect_manager_)

    status = aspect_manager_->popLast();
    CGRAPH_FUNCTION_CHECK_STATUS

    if (0 == aspect_manager_->getSize()) {
        CGRAPH_DELETE_PTR(aspect_manager_)
    }

    CGRAPH_FUNCTION_END
}


CStatus GElement::asyncRun() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!isAsync(), "[" + name_ + "] cannot async run.")

    async_result_ = thread_pool_->commit([this] {
        return run();
    }, CGRAPH_POOL_TASK_STRATEGY);

    auto futStatus = async_result_.wait_for(std::chrono::milliseconds(timeout_));
    if (std::future_status::ready == futStatus) {
        status = getAsyncResult();
    } else {
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION( GElementTimeoutStrategy::AS_ERROR == timeout_strategy_,    \
        "[" + name_ + "] running time more than [" + std::to_string(timeout_) + "]ms")
        cur_state_.store(GElementState::TIMEOUT, std::memory_order_release);
    }

    CGRAPH_FUNCTION_END
}


CStatus GElement::getAsyncResult() {
    CGRAPH_FUNCTION_BEGIN
    if (async_result_.valid()) {
        status = async_result_.get();    // 这里的get和valid方法，都是线程安全的
    }

    CGRAPH_FUNCTION_END
}


CStatus GElement::checkSuitable() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(thread_pool_)

    // 包含异步执行的逻辑，不可以loop超过1次
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(loop_ > CGRAPH_DEFAULT_LOOP_TIMES && this->isAsync(),     \
    "[" + this->getName() + "] can set loop <= 1 only for the reason of async run")
    if (!this->isRegistered()) {
        CGRAPH_ECHO("[notice] [%s] is created but not registered into pipeline, so it will not work.", this->getName().c_str());
    }

    CGRAPH_FUNCTION_END
}


GElementPtrArr GElement::getDeepPath(CBool reverse) const {
    GElementPtrArr path;
    auto* cur = const_cast<GElementPtr>(this);
    while (cur) {
        path.push_back(cur);
        cur = cur->belong_;
    }

    if (!reverse) {
        // 如果 reverse=false，则 pipeline(nullptr)->a->b->this
        std::reverse(path.begin(), path.end());
    }
    return path;
}


CBool GElement::isDefaultBinding() const {
    return CGRAPH_DEFAULT_BINDING_INDEX == binding_index_;
}

CGRAPH_NAMESPACE_END
