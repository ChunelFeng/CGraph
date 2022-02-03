/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemonObject.h
@Time: 2022/2/2 9:36 下午
@Desc: 
***************************/

#ifndef CGRAPH_GDAEMONOBJECT_H
#define CGRAPH_GDAEMONOBJECT_H

#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

class GDaemonObject : public GraphObject {

private:
    /**
     * 所有Daemon均不执行run方法
     * @return
     */
    CStatus run() final {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GDAEMONOBJECT_H
