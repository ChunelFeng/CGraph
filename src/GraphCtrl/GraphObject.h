/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphObject.h
@Time: 2021/10/2 9:58 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHOBJECT_H
#define CGRAPH_GRAPHOBJECT_H

#include "../CObject/CObjectInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GraphObject : public CObject {
public:
    CStatus run() override = 0;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GRAPHOBJECT_H
