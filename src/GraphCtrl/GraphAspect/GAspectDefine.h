/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectDefine.h
@Time: 2021/10/3 9:44 上午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTDEFINE_H
#define CGRAPH_GASPECTDEFINE_H

#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

enum class GAspectType {
    BEGIN_INIT = 0,
    FINISH_INIT = 1,
    BEGIN_RUN = 2,
    FINISH_RUN = 3,
    BEGIN_DESTROY = 4,
    FINISH_DESTROY = 5,
    ENTER_CRASHED = 99,
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECTDEFINE_H
