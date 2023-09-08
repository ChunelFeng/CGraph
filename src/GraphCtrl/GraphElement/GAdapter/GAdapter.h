/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAdapter.h
@Time: 2021/10/30 10:22 下午
@Desc:
***************************/

#ifndef CGRAPH_GADAPTER_H
#define CGRAPH_GADAPTER_H

#include "../GElement.h"

CGRAPH_NAMESPACE_BEGIN

class GAdapter : public GElement {
protected:
    explicit GAdapter() {
        element_type_ = GElementType::ADAPTER;
    }

    friend class GPipeline;
};

using GAdapterPtr = GAdapter *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GADAPTER_H
