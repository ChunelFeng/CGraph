/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStageObject.h
@Time: 2024/12/14 11:26
@Desc: 
***************************/

#ifndef CGRAPH_GSTAGEOBJECT_H
#define CGRAPH_GSTAGEOBJECT_H

#include <atomic>

#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

class GStageObject : public GraphObject {
private:
    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END


#endif //CGRAPH_GSTAGEOBJECT_H
