/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPassedDefaultParam.h
@Time: 2021/10/5 12:37 上午
@Desc:
***************************/

#ifndef CGRAPH_GPASSEDDEFAULTPARAM_H
#define CGRAPH_GPASSEDDEFAULTPARAM_H

#include "GPassedParam.h"

CGRAPH_NAMESPACE_BEGIN

class GPassedDefaultParam final : public GPassedParam {
protected:
    /**
     * 针对原生类型的实现
     * 目的是防止无法生成默认类型的 GAspectParam 参数
     * 无任何实意，不支持继承
     */
    CVoid clone(GPassedParamPtr param) final {
    }
};

using GAspectDefaultParam = GPassedDefaultParam;
using GDaemonDefaultParam = GPassedDefaultParam;
using GEventDefaultParam = GPassedDefaultParam;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPASSEDDEFAULTPARAM_H
