/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectDefaultParam.h
@Time: 2021/10/5 12:37 上午
@Desc:
***************************/

#ifndef CGRAPH_GASPECTDEFAULTPARAM_H
#define CGRAPH_GASPECTDEFAULTPARAM_H

#include "GAspectParam.h"

CGRAPH_NAMESPACE_BEGIN

class GAspectDefaultParam final : public GAspectParam {

protected:
    void clone(GAspectParamPtr param) final {
        /**
         * 针对原生类型的实现
         * 目的是防止无法生成默认类型的 GAspectParam 参数
         * 无任何实意，不支持继承
         */
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECTDEFAULTPARAM_H
