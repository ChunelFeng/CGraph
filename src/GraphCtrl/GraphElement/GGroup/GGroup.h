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
#include "../../GraphParam/GParamInclude.h"

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

    explicit GGroup();

    CStatus init() override;

    CStatus destroy() override;

    CBool isSerializable() override;

protected:
    GElementPtrArr group_elements_arr_;    // 存放 element的数组

    friend class GStaticEngine;
    friend class GPipeline;
};

using GGroupPtr = GGroup *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GGROUP_H
