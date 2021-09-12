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
#include "../../CObject/CObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

class GPipelineFactory : public CObject {

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
    static void destroy(GPipelinePtr pipeline);

    /**
     * 清空所有的pipeline信息
     */
    static void clear();

protected:
    CSTATUS run() override;
    GPipelineFactory() = default;

private:
    static GPipelinePtrList pipeline_list_;    // 记录所有的
    static std::mutex lock_;
};

#endif //CGRAPH_GPIPELINEFACTORY_H
