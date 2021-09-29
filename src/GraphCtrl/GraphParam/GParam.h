/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.h
@Time: 2021/6/11 6:32 下午
@Desc: 
***************************/


#ifndef CGRAPH_GPARAM_H
#define CGRAPH_GPARAM_H

#include <shared_mutex>
#include "GParamObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"
#include "GParamDefine.h"

class GParam : public GParamObject {
public:
    std::shared_mutex _param_shared_lock_;

private:
    /**
     * 每次pipeline执行结束，会调用一次reset
     * @return
     */
    virtual void reset() = 0;

    friend class GParamManager;
};

using GParamPtr = GParam *;

#endif //CGRAPH_GPARAM_H
