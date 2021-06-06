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

class GElementManager : public CObject {
public:
    explicit GElementManager();
    ~GElementManager() override;    // 注意，manager中的节点，在析构的时候不需要释放。所有的节点信息在CFlow类中统一申请和释放
    GElementManager(const GElementManager& manager);
    GElementManager& operator=(const GElementManager& manager);

    CSTATUS init() override;
    CSTATUS run() override;
    CSTATUS deinit() override;

protected:
    /**
     * 判定哪些节点是可以分到一个cluster中的
     * @return
     */
    CSTATUS preCheck();
    CSTATUS addElement(GElementPtr element);
    bool hasElement(GElementPtr element) const;
    void deleteElement(GElementPtr element);

private:
    GElementPtrSet manager_elements_;    // 保存节点信息的内容

    friend class GFlow;
    friend class GRegion;
};


#endif //CGRAPH_GELEMENTMANAGER_H
