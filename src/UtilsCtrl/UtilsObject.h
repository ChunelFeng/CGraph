/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UtilsObject.h
@Time: 2021/9/19 12:00 上午
@Desc: 
***************************/

#ifndef CGRAPH_UTILSOBJECT_H
#define CGRAPH_UTILSOBJECT_H

#include "../CObject/CObject.h"
#include "UtilsDefine.h"

CGRAPH_NAMESPACE_BEGIN

class UtilsObject : public CObject {

protected:
    CStatus run() override {
        // Utils类中的内容，默认均不支持 run() 方法
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTILSOBJECT_H
