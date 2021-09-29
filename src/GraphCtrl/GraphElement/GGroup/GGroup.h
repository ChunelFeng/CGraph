/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GGroup.h
@Time: 2021/6/15 1:49 下午
@Desc: 
***************************/


#ifndef CGRAPH_GGROUP_H
#define CGRAPH_GGROUP_H

#include <vector>
#include "GGroupDefine.h"
#include "../GElement.h"
#include "../../GraphParam/GParamInclude.h"

/* 所有节点组合的基类，所有节点组合功能，均继承自此类 */
class GGroup : public GElement {

public:
    virtual CSTATUS addElement(GElementPtr element) = 0;
};

using GGroupPtr = GGroup *;

#endif //CGRAPH_GGROUP_H
