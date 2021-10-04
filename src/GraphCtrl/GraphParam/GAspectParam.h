/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectParam.h
@Time: 2021/10/4 1:47 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTPARAM_H
#define CGRAPH_GASPECTPARAM_H

#include "GParamObject.h"

class GAspectParam : public GParamObject {
public:
    /**
     * 所有的 GAspectParam 类型，内部需要实现一个 clone 方法
     * 将外部函数传到 aspect 中
     * @param param
     */
    virtual void clone(GAspectParam* param) = 0;
};

using GAspectParamPtr = GAspectParam *;

#endif //CGRAPH_GASPECTPARAM_H
