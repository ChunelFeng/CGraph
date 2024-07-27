/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStaticEngine.h
@Time: 2024/7/27 10:40
@Desc: 
***************************/

#ifndef CGRAPH_GSTATICENGINE_H
#define CGRAPH_GSTATICENGINE_H

#include "../GEngine.h"

CGRAPH_NAMESPACE_BEGIN

class GStaticEngine : public GEngine {
protected:
    explicit GStaticEngine() = default;

    CStatus setup(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

private:
    GElementPtrMat2D element_mat_ {};        // 解图后的elements信息

    friend class UAllocator;
    friend class GElementManager;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTATICENGINE_H
