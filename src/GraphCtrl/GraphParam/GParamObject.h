/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamObject.h
@Time: 2021/9/29 9:23 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPARAMOBJECT_H
#define CGRAPH_GPARAMOBJECT_H

#include "../GraphObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GParamObject : public GraphObject {
protected:
    /**
     * GParam相关内容，无法通过run()方法执行
     * @return
     */
    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAMOBJECT_H
