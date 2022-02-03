/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GDaemon.cpp
@Time: 2022/2/3 1:19 下午
@Desc: 
***************************/

#include "GDaemon.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GDaemon::init() {
    CGRAPH_FUNCTION_BEGIN
    timer_.start(getInterval(), [&] {
        this->daemonRun();
    });
    CGRAPH_FUNCTION_END
}


CStatus GDaemon::destroy() {
    CGRAPH_FUNCTION_BEGIN
    timer_.stop();
    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END