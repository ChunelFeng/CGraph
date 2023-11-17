/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElement.cpp
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

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
    this->left_depend_ = dependence_.size();
}


CVoid GElement::afterRun() {
    for (auto& element : this->run_before_) {
        element->left_depend_--;
    }
    this->done_ = true;
}


GElementPtr GElement::setName(const std::string& name) {
    CGRAPH_ASSERT_INIT_THROW_ERROR(false)
    this->name_ = name.empty() ? this->session_ : name;

    // 设置name信息的时候，顺便给 aspect_manager_ 一起设置了
    if (aspect_manager_) {
        aspect_manager_->setName(name_);
    }
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


GElementRef GElement::operator--(int) noexcept {
    try {
        this->setVisible(true);
    } catch (const CException& ex) {
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
    } catch (const CException& ex) {
        CGRAPH_ECHO("[warning] default set loop failed.");
    }

    return (*this);
}


CBool GElement::isRunnable() const {
    return 0 >= this->left_depend_ && !this->done_;
}


CBool GElement::isLinkable() const {
    return this->linkable_;
}


CBool GElement::isAsync() const {
    // 如果timeout != 0, 则异步执行
    return this->timeout_ != CGRAPH_DEFAULT_ELEMENT_TIMEOUT;
}


CStatus GElement::addDependGElements(const GElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    if (!isMutable()) {
        // 如果是 mutable的逻辑，则可以在 init之后，修改依赖关系
        CGRAPH_ASSERT_INIT(false);
    }

    for (GElementPtr cur: elements) {
        CGRAPH_ASSERT_NOT_NULL(cur)
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((cur->belong_ != this->belong_),     \
        cur->getName() + " cannot depend because not same belong info")
        if (this == cur) {
            continue;
        }

        cur->run_before_.insert(this);
        this->dependence_.insert(cur);
    }

    this->left_depend_ = this->dependence_.size();
    CGRAPH_FUNCTION_END
}


CStatus GElement::setElementInfo(const GElementPtrSet& dependElements,
                                 const std::string& name,
                                 CSize loop,
                                 GParamManagerPtr paramManager,
                                 GEventManagerPtr eventManager) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    this->setName(name)->setLoop(loop);
    param_manager_ = paramManager;
    event_manager_ = eventManager;
    status = this->addDependGElements(dependElements);
    CGRAPH_FUNCTION_END
}


CStatus GElement::doAspect(const GAspectType& aspectType, const CStatus& curStatus) {
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
                if (0 == trigger_times_) {
                    /** 第一次执行的时候， */
                    status = prepareRun();
                    CGRAPH_FUNCTION_CHECK_STATUS
                }

                trigger_times_++;    // 记录实际上触发了多少次，而不是正式执行了多少次
                for (CSize i = 0; i < this->loop_ && GElementState::NORMAL == cur_state_.load(); i++) {
                    /** 执行带切面的run方法 */
                    status = doAspect(GAspectType::BEGIN_RUN);
                    CGRAPH_FUNCTION_CHECK_STATUS
                    do {
                        status = (!isAsync()) ? run() : asyncRun();
                        /**
                         * 在实际run结束之后，首先需要判断一下是否进入yield状态了。
                         * 接下来，如果状态是ok的，并且被条件hold住，则循环执行
                         * 默认所有element的isHold条件均为false，即不hold，即执行一次
                         * 可以根据需求，对任意element类型，添加特定的isHold条件
                         * */
                    } while (checkYield(), status.isOK() && this->isHold());
                    doAspect(GAspectType::FINISH_RUN, status);
                }
                break;
            }
            case CFunctionType::INIT: {
                status = doAspect(GAspectType::BEGIN_INIT);
                CGRAPH_FUNCTION_CHECK_STATUS
                status = init();
                doAspect(GAspectType::FINISH_INIT, status);
                break;
            }
            case CFunctionType::DESTROY: {
                status = doAspect(GAspectType::BEGIN_DESTROY);
                CGRAPH_FUNCTION_CHECK_STATUS
                status = destroy();
                doAspect(GAspectType::FINISH_DESTROY, status);
                break;
            }
            default:
                CGRAPH_RETURN_ERROR_STATUS("get function type error")
        }
    } catch (const CException& ex) {
        doAspect(GAspectType::ENTER_CRASHED);
        status = crashed(ex);
    }

    CGRAPH_FUNCTION_END
}


CStatus GElement::prepareRun() {
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
    CBool result = (GElementState::TIMEOUT == cur_state_.load());
    GElementPtr belong = this->belong_;
    while (!result && belong) {
        result = (GElementState::TIMEOUT == belong->cur_state_.load());
        belong = belong->belong_;
    }

    return result;
}


CBool GElement::isMutable() const {
    // 写入 GMutable的 element，属于 mutable，可以在运行时，修改依赖关系
    return belong_ && GElementType::MUTABLE == belong_->element_type_;
}


CStatus GElement::crashed(const CException& ex) {
    return CStatus(internal::STATUS_CRASH, ex.what(), CGRAPH_GET_LOCATE);
}


CIndex GElement::getThreadIndex() {
    CGRAPH_THROW_EXCEPTION_BY_CONDITION((nullptr == thread_pool_),    \
        this->getName() + " getThreadIndex with no threadpool")    // 理论不可能出现的情况

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
    if (src->isGroup() && dst->isGroup()) {
        // 在group的逻辑中，添加 cluster_ 的信息
        oss << 'p' << src << " -> p" << dst << label << "[ltail=cluster_p" << src << " lhead=cluster_p" << dst << "];\n";
    } else if (src->isGroup() && !dst->isGroup()) {
        oss << 'p' << src << " -> p" << dst << label << "[ltail=cluster_p" << src << "];\n";
    } else if (!src->isGroup() && dst->isGroup()) {
        oss << 'p' << src << " -> p" << dst << label << "[lhead=cluster_p" << dst << "];\n";
    } else {
        oss << 'p' << src << " -> p" << dst << label << ";\n";
    }
}


CVoid GElement::dumpElement(std::ostream& oss) {
    dumpElementHeader(oss);
    dumpPerfInfo(oss);

    oss << "\"];\n";
    if (this->loop_ > 1 && !this->isGroup()) {
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
        return GElementState::YIELD != cur_state_;
    });
}


CBool GElement::isGroup() const {
    // 按位与 GROUP有值，表示是 GROUP的逻辑
    return (long(element_type_) & long(GElementType::GROUP)) > 0;
}


CIndex GElement::getBindingIndex() {
    return this->binding_index_;
}


CStatus GElement::buildRelation(GElementRelation& relation) {
    CGRAPH_FUNCTION_BEGIN

    relation.predecessors_ = this->dependence_;    // 前驱
    relation.successors_ = this->run_before_;    // 后继
    relation.belong_ = this->belong_;    // 从属信息

    CGRAPH_FUNCTION_END
}


CBool GElement::isSerializable() {
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
        cur_state_.store(GElementState::TIMEOUT);
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
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION((loop_ > CGRAPH_DEFAULT_LOOP_TIMES && this->isAsync()),     \
    "[" + name_ + "] can set loop <= 1 only for the reason of async run")

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
