/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelineFactory.h
@Time: 2021/6/23 10:25 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINEFACTORY_H
#define CGRAPH_GPIPELINEFACTORY_H

#include <list>
#include <mutex>

#include "GPipeline.h"
#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

class GPipelineFactory : public GraphObject {
public:
    /**
     * 创建一个pipeline信息
     * @return
     */
    static GPipelinePtr create();

    /**
     * 销毁一个pipeline信息
     * @return
     */
    static CStatus remove(GPipelinePtr pipeline);

    /**
     * 清空所有的pipeline信息
     */
    static CStatus clear();

private:
    static GPipelinePtrList s_pipeline_list_;    // 记录所有的
    static std::mutex s_lock_;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINEFACTORY_H
