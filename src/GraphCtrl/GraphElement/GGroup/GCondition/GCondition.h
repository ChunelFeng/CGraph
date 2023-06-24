/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GCondition.h
@Time: 2021/6/19 5:00 下午
@Desc: 实现在多个element中，选择一个执行的功能
***************************/

#ifndef CGRAPH_GCONDITION_H
#define CGRAPH_GCONDITION_H

#include "../GGroup.h"

CGRAPH_NAMESPACE_BEGIN

class GCondition : public GGroup {
protected:
    explicit GCondition();

    /**
     * 计算需要返回第n个信息
     * 执行最后一个，返回-1即可。
     * 超出-1和size之间的范围，则不执行
     * @return
     */
    virtual CIndex choose() = 0;

    /**
     * 获取当前condition组内部元素的个数
     * @return
     */
    CSize getRange() const;

private:
    CVoid dump(std::ostream& oss) final;

    CStatus run() override;

    CStatus addElement(GElementPtr element) override;

    friend class GPipeline;
};

using GConditionPtr = GCondition *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GCONDITION_H
