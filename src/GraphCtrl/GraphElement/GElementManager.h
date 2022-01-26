/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementManager.h
@Time: 2021/6/2 10:33 下午
@Desc:
***************************/

#ifndef CGRAPH_GELEMENTMANAGER_H
#define CGRAPH_GELEMENTMANAGER_H

#include "GElement.h"
#include "GGroup/GCluster/GCluster.h"

CGRAPH_NAMESPACE_BEGIN

class GElementManager : public CObject {
protected:
    explicit GElementManager();
    ~GElementManager() override;    // 注意，manager中的节点，在析构的时候不需要释放。所有的节点信息在GPipeLine类中统一申请和释放
    GElementManager(const GElementManager& manager);
    GElementManager& operator=(const GElementManager& manager);

    CStatus init() override;
    CStatus run() override;
    CStatus destroy() override;

    /**
     * 判定哪些节点是可以分到一个cluster中的
     * @return
     */
    CStatus preRunCheck();

    /**
     * 将所有的节点，分发到para_cluster_arrs_中，运行的时候使用。
     * @return
     */
    CStatus analyse();

    /**
     * 执行完毕后，确认运行是否正常
     * 正常指的是，所有节点被运行loop次
     * @param runNodeSize
     * @return
     */
    CStatus afterRunCheck(int runNodeSize);

    CStatus addElement(GElementPtr element);
    bool hasElement(GElementPtr element) const;
    void deleteElement(GElementPtr element);


private:
    GElementPtrSet manager_elements_;                    // 保存节点信息的内容
    ParaWorkedClusterArrs para_cluster_arrs_;            // 可以并行的cluster数组

    friend class GPipeline;
    friend class GRegion;
    friend class UAllocator;
};

using GElementManagerPtr = GElementManager *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTMANAGER_H
