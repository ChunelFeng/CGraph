/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectManager.h
@Time: 2021/10/2 10:17 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTMANAGER_H
#define CGRAPH_GASPECTMANAGER_H

#include "../GraphManager.h"
#include "GAspectObject.h"
#include "GAspect.h"

CGRAPH_NAMESPACE_BEGIN

class GAspectManager : public GAspectObject,
                       public GraphManager<GAspect> {
protected:
    explicit GAspectManager();

    ~GAspectManager() override ;

    /**
     * 执行切面逻辑
     * @param type
     * @param curStatus
     * @return
     */
    CStatus reflect(const internal::GAspectType& type,
                    const CStatus& curStatus = CStatus());

    CStatus add(GAspectPtr aspect) final;

    CSize getSize() const final;

    CStatus clear() final;

    /**
     * 弹出去最后一个
     * @return
     */
    CStatus popLast();

    CVoidPtr setGParamManager(GParamManagerPtr pm) final;

    CVoidPtr setGEventManager(GEventManagerPtr em) final;

    CGRAPH_NO_ALLOWED_COPY(GAspectManager)

private:
    GAspectPtrArr aspect_arr_;     // 存储aspect的容器

    friend class GElement;
    friend class UAllocator;
};

using GAspectManagerPtr = GAspectManager *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GASPECTMANAGER_H
