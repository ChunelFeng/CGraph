/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelineManager.cpp
@Time: 2023/1/15 13:27
@Desc: 
***************************/

#include <algorithm>

#include "GPipelineManager.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GPipelineManager::init() {
    CGRAPH_FUNCTION_BEGIN
    if (!used_list_.empty()) {
        // 初始化的时候，不应该有 used 的内容
        CGRAPH_RETURN_ERROR_STATUS("used list status error")
    }

    for (auto pipeline : free_list_) {
        status += pipeline->init();
    }
    CGRAPH_FUNCTION_CHECK_STATUS
    is_init_ = true;
    CGRAPH_FUNCTION_END
}


CStatus GPipelineManager::run() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(true)
    GPipelinePtr pipeline = fetch();
    if (nullptr == pipeline) {
        CGRAPH_RETURN_ERROR_STATUS("no free pipeline current")
    }

    /**
     * 这里执行，是外部线程执行的。获取的status就是这个pipeline run的结果
     * 无论如何，执行完成之后，都要将pipeline返回到 free_list 中去
     */
    status = pipeline->run();
    status += release(pipeline);

    CGRAPH_FUNCTION_END
}


CStatus GPipelineManager::destroy() {
    CGRAPH_FUNCTION_BEGIN
    if (!used_list_.empty()) {
        CGRAPH_RETURN_ERROR_STATUS("used list status error")
    }

    for (auto pipeline : free_list_) {
        status += pipeline->destroy();
    }
    CGRAPH_FUNCTION_CHECK_STATUS

    is_init_ = false;
    CGRAPH_FUNCTION_END
}


CStatus GPipelineManager::add(GPipelinePtr ptr) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(ptr)
    CGRAPH_ASSERT_INIT(false)

    free_list_.push_back(ptr);
    CGRAPH_FUNCTION_END
}


CStatus GPipelineManager::clear() {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    /**
     * 这里只是清空list中的信息，并不真正销毁 pipeline对象
     * 因为pipeline 对象统一在 Factory中管控
     */
    used_list_.clear();
    free_list_.clear();
    CGRAPH_FUNCTION_END
}


CBool GPipelineManager::find(GPipelinePtr ptr) const {
    /**
     * 如果在其中一个位置，找到了，就返回 true了
     * 非线程安全，建议 init 之前使用
     */
    CBool result = std::any_of(used_list_.begin(), used_list_.end(),
                         [ptr] (GPipelinePtr cur) {
       return (ptr == cur);
    });

    result |= std::any_of(free_list_.begin(), free_list_.end(),
                          [ptr] (GPipelinePtr cur) {
       return (ptr == cur);
    });

    return result;
}


CStatus GPipelineManager::remove(GPipelinePtr ptr) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(ptr)
    CGRAPH_ASSERT_INIT(false)
    free_list_.remove(ptr);    // 在非初始化的情况下，仅可能在 free_list 中包含

    CGRAPH_FUNCTION_END
}


CSize GPipelineManager::getSize() const {
    CSize size = free_list_.size() + used_list_.size();
    return size;
}


GPipelineManager::~GPipelineManager() {
    clear();
}


GPipelinePtr GPipelineManager::fetch() {
    GPipelinePtr pipeline = nullptr;
    CGRAPH_LOCK_GUARD lk(mutex_);
    if (free_list_.empty()) {
        // 如果没有 free的了，则直接返回空了
        return pipeline;
    }

    pipeline = free_list_.front();
    free_list_.pop_front();    // 如果有的话，就fetch出去了
    used_list_.push_back(pipeline);
    return pipeline;
}


CStatus GPipelineManager::release(GPipelinePtr ptr) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(ptr)
    CGRAPH_LOCK_GUARD lk(mutex_);
    CBool result = std::any_of(used_list_.begin(), used_list_.end(),
                               [ptr] (GPipelinePtr cur) {
                                   return (ptr == cur);
                               });
    if (!result) {
        CGRAPH_RETURN_ERROR_STATUS("pipeline is not used")
    }

    used_list_.remove(ptr);
    free_list_.push_front(ptr);    // 最先执行的，放回头部，可能会增加cache触达

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
