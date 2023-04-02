/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelinePy.h
@Time: 2023/4/2 22:41
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINEPY_H
#define CGRAPH_GPIPELINEPY_H

#include "GPipeline.h"

CGRAPH_NAMESPACE_BEGIN

class GPipelinePy : public GPipeline {
public:
    /**
     * 注册一个节点信息
     * @param element
     * @param dependElements
     * @param name
     * @param loop
     * @return
     */
    CStatus registerGElement(GElementPtr element,
                             const GElementPtrSet &dependElements = std::initializer_list<GElementPtr>(),
                             const std::string &name = CGRAPH_EMPTY,
                             CSize loop = CGRAPH_DEFAULT_LOOP_TIMES);
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINEPY_H
