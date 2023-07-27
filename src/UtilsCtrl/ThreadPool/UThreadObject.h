/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CThreadObject.h
@Time: 2021/7/2 10:39 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTHREADOBJECT_H
#define CGRAPH_UTHREADOBJECT_H

#include "../UtilsObject.h"
#include "UThreadPoolDefine.h"

CGRAPH_NAMESPACE_BEGIN

class UThreadObject : public UtilsObject {
protected:
    /**
     * 部分thread中的算子，可以不实现run方法
     * @return
     */
    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTHREADOBJECT_H
