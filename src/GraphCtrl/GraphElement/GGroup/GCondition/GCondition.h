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

class GCondition : public GGroup {
protected:
    CSTATUS init() override;

    CSTATUS deinit() override;

    CSTATUS process(bool isMock) override;

    CSTATUS run() override;

    CSTATUS addElement(GElementPtr element) override;

    /**
     * 计算需要返回第n个信息
     * 执行最后一个，返回-1即可。
     * 超出-1和size之间的范围，则不执行
     * @return
     */
    virtual int choose() = 0;

private:
    GElementPtrArr condition_elements_;

    friend class GPipeline;
};

using GConditionPtr = GCondition *;


#endif //CGRAPH_GCONDITION_H
