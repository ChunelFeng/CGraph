/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageParam.h
@Time: 2022/10/30 23:01
@Desc: 
***************************/

#ifndef CGRAPH_GMESSAGEPARAM_H
#define CGRAPH_GMESSAGEPARAM_H

#include "GParamObject.h"

CGRAPH_NAMESPACE_BEGIN

class GMessageParam : public GParamObject {
    /**
     * 用于消息传递的参数
     * 建议派生类重载赋值构造函数
     * 否则可能会出现深浅拷贝的问题
     * @param param
     * @return
     */
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEPARAM_H
