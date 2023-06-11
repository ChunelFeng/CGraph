/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: DAnnNode.h
@Time: 2022/4/4 21:17
@Desc: ann领域算子模型抽象
***************************/

#ifndef CGRAPH_DANNNODE_H
#define CGRAPH_DANNNODE_H

#include "DAnnDefine.h"
#include "DAnnObject.h"

CGRAPH_NAMESPACE_BEGIN

class DAnnNode : public DAnnObject, public GNode {
protected:
    /**
     * 预处理参数信息，并且返回需要执行的函数信息
     * @return
     */
    virtual DAnnFuncType prepareParam() = 0;

    /**
     * 训练
     * @return
     */
    virtual CStatus train();

    /**
     * 查询
     * @return
     */
    virtual CStatus search();

    /**
     * 插入
     * @return
     */
    virtual CStatus insert();

    /**
     * 修改
     * @return
     */
    virtual CStatus update();

    /**
     * 删除
     * @return
     */
    virtual CStatus remove();

    /**
     * 加载模型信息
     * @return
     */
    virtual CStatus loadModel();

    /**
     * 保存模型信息
     * @return
     */
    virtual CStatus saveModel();

    /**
     * 其他方法
     */
    virtual CStatus edition();

    /**
     * 将参数更新，作用于所有功能函数之后
     * @return
     */
    virtual CStatus refreshParam();

protected:
    /**
     * 构造函数信息
     */
    explicit DAnnNode();

    CGRAPH_NO_ALLOWED_COPY(DAnnNode)

    /**
     * 分拆执行函数
     * @return
     */
    CStatus run() override;

private:
    typedef	CStatus (DAnnNode::*DAnnNodeFuncPtr)();
    DAnnNodeFuncPtr ann_func_arr_[static_cast<CUint>(DAnnFuncType::ANN_MAX_SIZE)]{};    // ann函数映射关系
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_DANNNODE_H
