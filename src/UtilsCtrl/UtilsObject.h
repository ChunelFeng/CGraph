/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UtilsObject.h
@Time: 2021/9/19 12:00 上午
@Desc: 
***************************/

#ifndef CGRAPH_UTILSOBJECT_H
#define CGRAPH_UTILSOBJECT_H

#include "UtilsDefine.h"

CGRAPH_NAMESPACE_BEGIN

class UtilsObject : public CObject {
public:
    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSOBJECT_H
