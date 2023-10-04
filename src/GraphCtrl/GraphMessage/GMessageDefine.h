/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageDefine.h
@Time: 2022/12/24 22:16
@Desc: 
***************************/

#ifndef CGRAPH_GMESSAGEDEFINE_H
#define CGRAPH_GMESSAGEDEFINE_H

#include "GMessageObject.h"

CGRAPH_NAMESPACE_BEGIN

using GMessagePushStrategy = URingBufferPushStrategy;

static const char* PUB_SUB_PREFIX = "PS_PRE_";
static const char* SEND_RECV_PREFIX = "SR_PRE_";

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEDEFINE_H
