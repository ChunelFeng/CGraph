/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GEvent.cpp
@Time: 2023/1/20 23:07
@Desc: 
***************************/

#include "GEvent.h"

CGRAPH_NAMESPACE_BEGIN

GEvent::GEvent() {
    session_ = URandom<>::generateSession(CGRAPH_STR_EVENT);
}


GEvent::~GEvent() {
    CGRAPH_DELETE_PTR(param_)
}


CStatus GEvent::init() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)
    async_run_finish_futures_.clear();
    async_destroy_futures_.clear();
    is_init_ = true;

    CGRAPH_FUNCTION_END
}


CStatus GEvent::destroy() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)

    asyncWait(GEventAsyncStrategy::PIPELINE_DESTROY);
    async_run_finish_futures_.clear();
    async_destroy_futures_.clear();
    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GEvent::process(GEventType type, GEventAsyncStrategy strategy) {
    CGRAPH_FUNCTION_BEGIN

    switch (type) {
        case GEventType::SYNC:    // 同步触发
            this->trigger(this->param_);
            break;
        case GEventType::ASYNC:    // 异步触发
            CGRAPH_ASSERT_NOT_NULL(this->thread_pool_)
            {
                auto future = thread_pool_->commit([this] {
                    this->trigger(this->param_);
                }, CGRAPH_POOL_TASK_STRATEGY);

                /**
                 * 根据具体策略，将 future信息放到对应的容器中
                 * 在特定的时间点，等待执行结束
                 */
                if (GEventAsyncStrategy::PIPELINE_RUN_FINISH == strategy) {
                    async_run_finish_futures_.emplace_back(std::move(future));
                } else if (GEventAsyncStrategy::PIPELINE_DESTROY == strategy) {
                    async_destroy_futures_.emplace_back(std::move(future));
                }
            }
            break;
        default:
            CGRAPH_RETURN_ERROR_STATUS("unknown event type")
    }

    CGRAPH_FUNCTION_END
}


CVoid GEvent::asyncWait(GEventAsyncStrategy strategy) {
    switch (strategy) {
        case GEventAsyncStrategy::PIPELINE_RUN_FINISH: {
            for (auto& cur : async_run_finish_futures_) {
                if (cur.valid()) {
                    cur.wait();
                }
            }
            async_run_finish_futures_.clear();
        }
            break;
        case GEventAsyncStrategy::PIPELINE_DESTROY: {
            for (auto& cur : async_destroy_futures_) {
                if (cur.valid()) {
                    cur.wait();
                }
            }
            async_destroy_futures_.clear();
        }
            break;
        default:
            CGRAPH_THROW_EXCEPTION("unknown event async strategy type")
    }
}

CGRAPH_NAMESPACE_END