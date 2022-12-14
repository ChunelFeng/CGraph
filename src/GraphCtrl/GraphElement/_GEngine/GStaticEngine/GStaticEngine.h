/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStaticEngine.h
@Time: 2022/12/11 16:34
@Desc: 
***************************/

#ifndef CGRAPH_GSTATICENGINE_H
#define CGRAPH_GSTATICENGINE_H

#include "../GEngine.h"
#include "../../GGroup/GCluster/GCluster.h"

CGRAPH_NAMESPACE_BEGIN

class GStaticEngine : public GEngine {
protected:
    CStatus setUp(const GSortedGElementPtrSet& elements) override;

    CStatus run() override;

    CStatus afterRunCheck() override;

    /**
     * 对数据进行标记和整理
     * @param elements
     * @return
     */
    CStatus mark(const GSortedGElementPtrSet& elements);

    /**
     * 将所有注册到 pipeline 中的信息，解析到 para_cluster_arrs_ 中
     * @param elements
     * @return
     */
    CStatus analyse(const GSortedGElementPtrSet& elements);

private:
    ParaWorkedClusterArrs para_cluster_arrs_;        // 可以并行的cluster数组
    CUint run_element_size_ = 0;                     // 当前已经执行的element的数量
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTATICENGINE_H
