/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageManagerSingleton.h
@Time: 2022/10/31 23:23
@Desc: 
***************************/

#ifndef CGRAPH_GMESSAGEMANAGERSINGLETON_H
#define CGRAPH_GMESSAGEMANAGERSINGLETON_H

#include "GMessage.h"
#include "GMessageManager.h"

CGRAPH_NAMESPACE_BEGIN

static USingleton<GMessageManager<>, USingletonType::LAZY, false> GMessageManagerSingleton;
static auto GMessageManagerSingletonPtr = GMessageManagerSingleton.get();

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEMANAGERSINGLETON_H
