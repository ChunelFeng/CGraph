/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSchedule.cpp
@Time: 2023/3/25 19:01
@Desc: 
***************************/

#include "GSchedule.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GSchedule::init() {
    CGRAPH_FUNCTION_BEGIN

    if (GScheduleType::UNIQUE == type_) {
        /**
         * 如果是 unique 的数据，则在这里初始化一下
         * 如果是 shared 的数据，则不做任何操作
         */
        CGRAPH_DELETE_PTR(unique_tp_)
        unique_tp_ = UAllocator::safeMallocTemplateCObject<UThreadPool>(true, config_);
    } else if (GScheduleType::SHARED == type_) {
        CGRAPH_ASSERT_NOT_NULL(shared_tp_)
        // 首先，要确定外部传入的线程池，已经初始化过了
        if (!shared_tp_->isInit()) {
            CGRAPH_RETURN_ERROR_STATUS("shared thread pool is not init.")
        }
    }

    CGRAPH_FUNCTION_END
}


CStatus GSchedule::destroy() {
    CGRAPH_FUNCTION_BEGIN

    if (GScheduleType::UNIQUE == type_) {
        // unique 模式下，init的时候创建线程，就在destroy的时候，给析构掉了
        CGRAPH_DELETE_PTR(this->unique_tp_)
    }

    CGRAPH_FUNCTION_END
}


CStatus GSchedule::run() {
    CGRAPH_NO_SUPPORT
}


CStatus GSchedule::makeShared(UThreadPoolPtr tp) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(tp)
    if (GScheduleType::SHARED == type_) {
        // 如果已经设置过了，就无法再设置了，防止异常情况出现
        CGRAPH_RETURN_ERROR_STATUS("cannot set schedule again.")
    }

    CGRAPH_DELETE_PTR(this->unique_tp_)
    shared_tp_ = tp;    // 用外部传入的线程池，来代替内部原有的线程池

    // 默认是 unique的，设置了之后就是 shared的了
    type_ = GScheduleType::SHARED;
    CGRAPH_FUNCTION_END
}


UThreadPoolPtr GSchedule::getThreadPool() {
    /**
     * 返回一个可用的 pool
     * utp和stp，应该是有且仅有一个为非空的
     */
    return unique_tp_ == nullptr ? shared_tp_ : unique_tp_;
}


GSchedule::~GSchedule() {
    destroy();
}

CGRAPH_NAMESPACE_END
