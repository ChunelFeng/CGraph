/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTemplateAspect.h
@Time: 2022/12/23 22:01
@Desc: 用于生成可变构造参数的切面
***************************/

#ifndef CGRAPH_GTEMPLATEASPECT_H
#define CGRAPH_GTEMPLATEASPECT_H

#include "GAspect.h"

CGRAPH_NAMESPACE_BEGIN

template<class ...Args>
class GTemplateAspect : public GAspect {
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTEMPLATEASPECT_H
