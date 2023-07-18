/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: DAnnObject.h
@Time: 2022/4/4 21:17
@Desc: 所有的Ann抽象接口类
***************************/

#ifndef CGRAPH_DANNOBJECT_H
#define CGRAPH_DANNOBJECT_H

#include "../DomainObject.h"

CGRAPH_NAMESPACE_BEGIN

class DAnnObject : public DomainObject {
protected:
    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_DANNOBJECT_H
