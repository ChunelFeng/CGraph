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

class UtilsObject : public CObject {

protected:
    CSTATUS run() override {
        // Utils类中的内容，默认均不支持 run() 方法
        CGRAPH_NO_SUPPORT
    }
};

#endif //CGRAPH_UTILSOBJECT_H
