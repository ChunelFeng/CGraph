/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelineManager.h
@Time: 2023/1/15 12:44
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINEMANAGER_H
#define CGRAPH_GPIPELINEMANAGER_H

#include <list>
#include <mutex>

#include "GPipelineObject.h"
#include "GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

class GPipelineManager : public GPipelineObject,
                         public GraphManager<GPipeline> {
public:
    CStatus init() override;

    CStatus run() override;

    CStatus destroy() override;

    CStatus add(GPipelinePtr ptr) override;

    CStatus clear() final;

    CBool find(GPipelinePtr ptr) const override;

    CStatus remove(GPipelinePtr ptr) override;

    CSize getSize() const override;

    ~GPipelineManager() override;

    /**
     * 获取一个可以运行的pipeline信息
     * 没有可用的话，则返回 nullptr
     * @return
     */
    GPipelinePtr fetch();

    /**
     * 释放一个pipeline信息，重新放回 free list 中去
     * @param ptr
     * @return
     */
    CStatus release(GPipelinePtr ptr);

private:
    GPipelinePtrList free_list_;                // 空闲的 pipeline信息
    GPipelinePtrList used_list_;                // 被用的 pipeline信息
    std::mutex mutex_;                          // 控制 free 和 used 之间转移的互斥量
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINEMANAGER_H
