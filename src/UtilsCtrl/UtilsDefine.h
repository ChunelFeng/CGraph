/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UtilsDefine.h
@Time: 2021/4/30 8:52 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTILSDEFINE_H
#define CGRAPH_UTILSDEFINE_H

#include "../CObject/CObject.h"

/* 开启函数流程 */
#define CGRAPH_FUNCTION_BEGIN           \
    CSTATUS status = STATUS_OK;         \

/* 结束函数流程 */
#define CGRAPH_FUNCTION_END             \
    return status;                      \

/* 判断传入的指针信息是否为空 */
#define CGRAPH_ASSERT_NOT_NULL(ptr)     \
    if (nullptr == (ptr)) {             \
        return STATUS_RES;              \
    }                                   \

/* 判断函数流程是否可以继续 */
#define CGRAPH_FUNCTION_CHECK_STATUS    \
    if (STATUS_OK != status) {          \
        return status;                  \
    }                                   \


#endif //CGRAPH_UTILSDEFINE_H
