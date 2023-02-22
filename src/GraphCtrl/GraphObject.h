/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphObject.h
@Time: 2021/10/2 9:58 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHOBJECT_H
#define CGRAPH_GRAPHOBJECT_H

#include "../CBasic/CBasicInclude.h"
#include "../UtilsCtrl/UtilsInclude.h"
#include "GraphDefine.h"

CGRAPH_NAMESPACE_BEGIN

class GraphObject : public CObject {
protected:
    CBool is_init_ = false;                          // 判断是否init
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GRAPHOBJECT_H
