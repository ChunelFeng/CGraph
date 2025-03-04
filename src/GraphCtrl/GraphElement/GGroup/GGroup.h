/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GGroup.h
@Time: 2021/6/15 1:49 下午
@Desc: 
***************************/

#ifndef CGRAPH_GGROUP_H
#define CGRAPH_GGROUP_H

#include <vector>

#include "GGroupDefine.h"
#include "../GElement.h"

CGRAPH_NAMESPACE_BEGIN

/* 所有节点组合的基类，所有节点组合功能，均继承自此类 */
class GGroup : public GElement {
protected:
    CBool isSerializable() const override;

    explicit GGroup();

private:
    /**
     * 向group中，添加element信息
     * @param element
     * @return
     */
    CStatus addElement(GElementPtr element);

    /**
     * 在添加element的时候，附加选项
     * @param element
     * @return
     */
    virtual CStatus addElementEx(GElementPtr element);

    CStatus addManagers(GParamManagerPtr paramManager,
                        GEventManagerPtr eventManager,
                        GStageManagerPtr stageManager) override;

    CStatus init() override;

    CStatus destroy() override;

    /**
     * 生成graphviz中 group对应的label 的开头信息
     * @param oss
     * @return
     */
    CVoid dumpGroupLabelBegin(std::ostream& oss);

    /**
     * 生成graphviz中的 group对应的label 的结尾信息
     * @param oss
     * @return
     */
    CVoid dumpGroupLabelEnd(std::ostream& oss);

    /**
     * 判断两个element，是否相互独立
     * @param a
     * @param b
     * @return
     * @notice 如果返回 true，则表示一定有前后依赖关系，不可能并发执行。否则表示不确定，原因是并发情况无法完全确定
     */
    virtual CBool isSeparate(GElementCPtr a, GElementCPtr b) const;

private:
    GElementPtrArr group_elements_arr_;    // 存放 element的数组

    friend class GPipeline;
    friend class GCluster;
    friend class GRegion;
    friend class GCondition;
    friend class GMutable;
    template<GMultiConditionType> friend class GMultiCondition;
    template<CInt> friend class GSome;

public:
    CStatus __addElements_4py(const GElementPtrArr& elements);
};

using GGroupPtr = GGroup *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GGROUP_H
