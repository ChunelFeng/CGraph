/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GPipelineObject.h
@Time: 2023/1/12 23:12
@Desc: 
***************************/

#ifndef CGRAPH_GPIPELINEOBJECT_H
#define CGRAPH_GPIPELINEOBJECT_H

#include "../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

class GPipelineObject : public GraphObject {
protected:
    CBool is_init_ = false;                          // 判断是否init
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPIPELINEOBJECT_H
