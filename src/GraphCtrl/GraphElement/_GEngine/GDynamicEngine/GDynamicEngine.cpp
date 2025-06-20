/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GDynamicEngine.cpp
@Time: 2022/12/16 22:46 下午
@Desc: 
***************************/

#include "GDynamicEngine.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GDynamicEngine::setup(const GSortedGElementPtrSet& elements) {
    CGRAPH_FUNCTION_BEGIN
    /**
     * 1. 判断是否是 dag 结构
     * 2. 标记数据，比如有多少个结束element等
     * 3. 计算element的结构类型
     * 4. 分析当前dag类型信息
     */
    CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!GEngine::isDag(elements),
                                            "it is not a dag struct");
    mark(elements);
    calcShape(elements);
    analysisDagType(elements);
    CGRAPH_FUNCTION_END
}


CStatus GDynamicEngine::run() {
    cur_status_.reset();

    switch (dag_type_) {
        case internal::GEngineDagType::COMMON:
            commonRunAll();
            break;
        case internal::GEngineDagType::ALL_SERIAL:
            serialRunAll();
            break;
        case internal::GEngineDagType::ALL_PARALLEL:
            parallelRunAll();
            break;
        default:
            CGRAPH_RETURN_ERROR_STATUS("unknown engine dag type");
    }

    return cur_status_;
}


CVoid GDynamicEngine::commonRunAll() {
    /**
     * 1. 执行没有任何依赖的element
     * 2. 在element执行完成之后，进行裂变，直到所有的element执行完成
     * 3. 等待异步执行结束
     */
    finished_end_size_ = 0;
    for (const auto& element : front_element_arr_) {
        process(element, element == front_element_arr_.back());
    }

    fatWait();
}


CVoid GDynamicEngine::mark(const GSortedGElementPtrSet& elements) {
    total_element_arr_.clear();
    front_element_arr_.clear();
    total_end_size_ = 0;

    for (GElementPtr element : elements) {
        if (element->run_before_.empty()) {
            total_end_size_++;
        }

        if (element->dependence_.empty()) {
            front_element_arr_.emplace_back(element);
        }
        total_element_arr_.emplace_back(element);
    }
}


CVoid GDynamicEngine::analysisDagType(const GSortedGElementPtrSet& elements) {
    if (total_element_arr_.empty()
       || (front_element_arr_.size() == 1 && total_element_arr_.size() - 1 == linked_size_)) {
        /**
         * 如果所有的信息中，只有一个是非linkable。则说明只有最后的那个是的，且只有一个开头
         * 故，这里将其认定为一条 lineal 的情况
         * ps: 只有一个或者没有 element的情况，也会被算到 ALL_SERIAL 中去
         */
        dag_type_ = internal::GEngineDagType::ALL_SERIAL;
    } else if (total_element_arr_.size() == total_end_size_ && front_element_arr_.size() == total_end_size_) {
        dag_type_ = internal::GEngineDagType::ALL_PARALLEL;
        analysisParallelMatrix();
    } else {
        dag_type_ = internal::GEngineDagType::COMMON;
    }
}


CVoid GDynamicEngine::analysisParallelMatrix() {
    parallel_element_matrix_.clear();
    const auto& config = thread_pool_->getConfig();
    CSize thdSize = config.default_thread_size_ + config.secondary_thread_size_;
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(thdSize <= 0,
                                        "default thread size cannot smaller than 1");
    CSize taskNumPerThd = total_end_size_ / thdSize + (CSize)(0 != total_end_size_ % thdSize);
    CGRAPH_THROW_EXCEPTION_BY_CONDITION(taskNumPerThd == 0,
                                        "task number per thread is 0");

    CSize curIndex = 0;
    while (curIndex < total_end_size_) {
        CSize curEnd = curIndex + taskNumPerThd < total_end_size_ ? curIndex + taskNumPerThd : total_end_size_ ;
        GElementPtrArr curArr(total_element_arr_.data() + curIndex, total_element_arr_.data() + curEnd);
        CGRAPH_THROW_EXCEPTION_BY_CONDITION(curArr.empty(),
                                            "current elements array cannot be empty");
        parallel_element_matrix_.push_back(curArr);
        curIndex += taskNumPerThd;
    }
}


CVoid GDynamicEngine::process(GElementPtr element, CBool affinity) {
    if (unlikely(cur_status_.isErr())) {
        return;
    }

    if (affinity && element->isDefaultBinding()) {
        // 如果 affinity=true，表示用当前的线程，执行这个逻辑。以便增加亲和性
        innerExec(element);
    } else {
        thread_pool_->execute([this, element] {
            this->innerExec(element); }, element->binding_index_);
    }
}


CVoid GDynamicEngine::innerExec(GElementPtr element) {
    element->refresh();
    const CStatus& curStatus = element->fatProcessor(CFunctionType::RUN);
    if (likely(curStatus.isNotErr())) {
        afterElementRun(element);
    } else {
        // 遇到异常情况，结束整体逻辑
        CGRAPH_LOCK_GUARD lk(status_lock_);
        cur_status_ += curStatus;
        locker_.cv_.notify_one();
    }
}


CVoid GDynamicEngine::afterElementRun(GElementPtr element) {
    element->done_ = true;
    if (unlikely(cur_status_.isErr())) {
        return;
    }

    switch (element->shape_) {
        case internal::GElementShape::NORMAL:
            {
                GElementPtr reserved = nullptr;
                for (auto* cur : element->run_before_) {
                    if (--cur->left_depend_ <= 0) {
                        if (reserved) {
                            process(cur, false);
                        } else {
                            reserved = cur;    // 留一个作为亲和性的，在当前线程运行
                        }
                    }
                }

                if (reserved) { process(reserved, true); }
            }
            break;
        case internal::GElementShape::LINKABLE:
            process(element->run_before_.front(), true);
            break;
        case internal::GElementShape::ROOT:
            for (auto* cur : element->run_before_) {
                process(cur, element->run_before_.back() == cur);
            }
            break;
        case internal::GElementShape::TAIL:
            {
                CGRAPH_LOCK_GUARD lock(locker_.mtx_);
                if ((++finished_end_size_ >= total_end_size_)) {
                    locker_.cv_.notify_one();
                }
            }
            break;
        default:
            CGRAPH_LOCK_GUARD lk(status_lock_);
            cur_status_.setErrorInfo("element shape type error");
            break;
    }
}


CVoid GDynamicEngine::fatWait() {
    CGRAPH_UNIQUE_LOCK lock(locker_.mtx_);
    locker_.cv_.wait(lock, [this] {
        /**
         * 遇到以下条件之一，结束执行：
         * 1，执行结束
         * 2，状态异常
         */
        return (finished_end_size_ >= total_end_size_) || cur_status_.isErr();
    });
}


#ifdef _CGRAPH_PARALLEL_MICRO_BATCH_ENABLE_
CVoid GDynamicEngine::parallelRunAll() {
    // 微任务模式，主要用于性能测试的场景下
    std::vector<std::future<CStatus>> futures;
    for (auto& elements : parallel_element_matrix_) {
        auto curFut = thread_pool_->commit([elements] {
            CGRAPH_FUNCTION_BEGIN
            for (auto* element : elements) {
                status += element->fatProcessor(CFunctionType::RUN);
                CGRAPH_FUNCTION_CHECK_STATUS
            }
            CGRAPH_FUNCTION_END;
        });
        futures.emplace_back(std::move(curFut));
    }

    for (auto& fut : futures) {
        cur_status_ += fut.get();
    }
}
#else
CVoid GDynamicEngine::parallelRunAll() {
    parallel_run_num_ = 0;
    for (CIndex i = 0; i < (CIndex)parallel_element_matrix_.size(); i++) {
        auto& curArr = parallel_element_matrix_[i];
        if (curArr.size() > 1) {
            for (const auto& element : curArr) {
                thread_pool_->executeWithTid([this, element] { parallelRunOne(element); }, i,
                                             element == curArr.front() || element == curArr.back(),
                                             element == curArr.front());
            }
        } else {
            // 仅有一个任务的情况，无法使用 executeWithTid 函数，故走这边的逻辑
            const auto& element = curArr.front();
            thread_pool_->execute([this, element] {
                parallelRunOne(element); }, element->binding_index_);
        }
    }

    if (parallel_element_matrix_.size() < (CSize)(thread_pool_->getConfig().default_thread_size_)) {
        // 确保所有的 pt 都可以被唤醒，从而快速执行
        thread_pool_->wakeupAllThread();
    }

    {
        CGRAPH_UNIQUE_LOCK lock(locker_.mtx_);
        locker_.cv_.wait(lock, [this] {
            return (parallel_run_num_ >= total_end_size_ || cur_status_.isErr());
        });
    }
}
#endif


CVoid GDynamicEngine::parallelRunOne(GElementPtr element) {
    if (unlikely(cur_status_.isErr())) {
        return;
    }

    auto status = element->fatProcessor(CFunctionType::RUN);
    if (unlikely(status.isErr())) {
        {
            CGRAPH_LOCK_GUARD lk(status_lock_);
            cur_status_ += status;
        }
    }

    auto finishedSize = parallel_run_num_.fetch_add(1, std::memory_order_release) + 1;
    if (finishedSize >= total_end_size_ || cur_status_.isErr()) {
        CGRAPH_UNIQUE_LOCK lock(locker_.mtx_);
        locker_.cv_.notify_one();
    }
}


CVoid GDynamicEngine::serialRunAll() {
    /**
     * 如果分析出来 dag是一个链式的，则直接依次执行element
     * 直到所有element都执行完成，或者有出现错误的返回值
     */
    GElementPtr cur = (front_element_arr_.empty()) ? nullptr : front_element_arr_.front();
    while (cur) {
        cur_status_ += cur->fatProcessor(CFunctionType::RUN);
        if (cur->run_before_.empty() || cur_status_.isErr()) {
            break;
        }
        cur = *(cur->run_before_.begin());
    }
}

CGRAPH_NAMESPACE_END
