/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSchedule.h
@Time: 2023/3/25 19:01
@Desc: 
***************************/

#ifndef CGRAPH_GSCHEDULE_H
#define CGRAPH_GSCHEDULE_H

#include "../../GraphObject.h"
#include "GScheduleDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GSchedule : public GraphObject {
private:
    /**
     * 设置调度资源模式
     * @param tp
     * @return
     */
    CStatus makeType(UThreadPoolPtr tp);

    /**
     * 获取线程池信息
     * @return
     */
    UThreadPoolPtr getThreadPool();

    CStatus init() final;

    CStatus run() final;

    CStatus destroy() final;

    explicit GSchedule() = default;

    ~GSchedule() override;

private:
    UThreadPoolPtr unique_tp_ = nullptr;                                  // 内部独占的线程池
    UThreadPoolPtr shared_tp_ = nullptr;                                  // 外部输入的线程池
    UThreadPoolConfig config_;                                            // 线程池的配置信息
    internal::GScheduleType type_ = internal::GScheduleType::UNIQUE;      // 调度类型

    friend class GPipeline;
    friend class GStorage;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSCHEDULE_H
