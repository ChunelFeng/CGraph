/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GBlock.h
@Time: 2021/6/15 1:49 下午
@Desc: 
***************************/


#ifndef CGRAPH_GBLOCK_H
#define CGRAPH_GBLOCK_H

#include <vector>
#include "../GElement.h"

/* 所有节点组合的基类，所有节点组合功能，均继承自此类 */
class GBlock : public GElement {
public:
    explicit GBlock();
    ~GBlock() override;

    virtual CSTATUS addElement(GElementPtr element) = 0;
};

using GBlockPtr = GBlock *;

#endif //CGRAPH_GBLOCK_H
