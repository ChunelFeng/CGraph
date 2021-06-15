/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSegment.h
@Time: 2021/6/15 1:49 下午
@Desc: 
***************************/


#ifndef CGRAPH_GSEGMENT_H
#define CGRAPH_GSEGMENT_H

#include <vector>
#include "../GElement.h"

/* 所有节点组合的基类，所有节点组合功能，均继承自此类 */
class GSegment : public GElement {
public:
    explicit GSegment();
    ~GSegment() override;

    virtual CSTATUS addElement(GElementPtr element) = 0;
};

using GBlockPtr = GSegment *;

#endif //CGRAPH_GSEGMENT_H
