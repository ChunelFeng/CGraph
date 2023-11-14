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
    /**
     * 向group中，添加element信息
     * @param element
     * @return
     */
    virtual CStatus addElement(GElementPtr element) = 0;

    CBool isSerializable() override;

    /**
     * 判断当前group是否已经被注册到特定pipeline中了。避免反复注册的问题
     * @return
     */
    CBool isRegistered();

private:
    explicit GGroup();

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

private:
    GElementPtrArr group_elements_arr_;    // 存放 element的数组

    friend class GStaticEngine;
    friend class GPipeline;
    friend class GCluster;
    friend class GRegion;
    friend class GCondition;
    friend class GMutable;
    template<GMultiConditionType> friend class GMultiCondition;
    template<CInt> friend class GSome;
};

using GGroupPtr = GGroup *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GGROUP_H
