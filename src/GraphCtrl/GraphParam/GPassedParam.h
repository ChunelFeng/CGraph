/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPassedParam.h
@Time: 2021/10/4 1:47 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPASSEDPARAM_H
#define CGRAPH_GPASSEDPARAM_H

#include <unordered_map>

#include "GParamObject.h"

CGRAPH_NAMESPACE_BEGIN

class GPassedParam : public GParamObject {
public:
    /**
     * 所有的 GPassedParam 类型，内部需要实现一个 clone 方法
     * 将外部函数传到内部，如aspect和daemon
     * @param param
     */
    virtual CVoid clone(GPassedParam* param) = 0;
};


/**
 * 这种类型的参数，可以用于aspect和daemon中，方便参数传递
 * 不同的地方，用不同的名字，对应的内容是同一个
 */
using GAspectParam = GPassedParam;
using GDaemonParam = GPassedParam;
using GElementParam = GPassedParam;
using GPassedParamPtr = GPassedParam *;
using GAspectParamPtr = GAspectParam *;
using GDaemonParamPtr = GDaemonParam *;
using GElementParamPtr = GElementParam *;

using GElementParamMap = std::unordered_map<std::string, GElementParamPtr>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPASSEDPARAM_H
