/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.h
@Time: 2021/6/11 6:32 下午
@Desc: 
***************************/

#ifndef CGRAPH_GPARAM_H
#define CGRAPH_GPARAM_H

#include <string>
#include <shared_mutex>

#include "GParamObject.h"
#include "../../UtilsCtrl/UtilsInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GParam : public GParamObject {
public:
    std::shared_mutex _param_shared_lock_;    // 用于参数互斥的锁信息

    /**
     * 保存参数信息
     * @param path 位置路径
     * @return
     */
    virtual CStatus dump(const std::string& path) {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 加载参数信息
     * @param path 位置路径
     * @return
     */
    virtual CStatus load(const std::string& path) {
        CGRAPH_NO_SUPPORT
    }

protected:
    /**
     * 每次pipeline执行结束，会调用一次reset
     * @return
     */
    virtual CVoid reset() = 0;

    friend class GParamManager;
};

using GParamPtr = GParam *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GPARAM_H
