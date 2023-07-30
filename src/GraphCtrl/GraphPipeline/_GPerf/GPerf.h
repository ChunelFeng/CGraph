/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPerf.h
@Time: 2023/7/29 22:38
@Desc: 
***************************/

#ifndef CGRAPH_GPERF_H
#define CGRAPH_GPERF_H

#include "../../GraphObject.h"
#include "GPerfAspect.h"
#include "GPerfDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GPipeline;

class GPerf : public GraphObject {
protected:
    explicit GPerf() = default;

    /**
     * 针对pipeline进行性能分析
     * @param pipeline
     * @return
     */
    static CStatus perf(GPipeline* pipeline);

    /**
     * 注入对应的切面信息
     * @param pipeline
     * @return
     */
    static CStatus inject(GPipeline* pipeline);

    friend class GPipeline;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPERF_H
