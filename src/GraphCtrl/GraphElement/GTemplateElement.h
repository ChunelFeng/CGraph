/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTemplateElement.h
@Time: 2022/12/24 10:54
@Desc: 
***************************/

#ifndef CGRAPH_GTEMPLATEELEMENT_H
#define CGRAPH_GTEMPLATEELEMENT_H

#include "GElement.h"

CGRAPH_NAMESPACE_BEGIN

template<typename ...Args>
class GTemplateElement : public GElement {
};

template<typename ...Args>
using GTemplateElementPtr = GTemplateElement<Args ...> *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTEMPLATEELEMENT_H
