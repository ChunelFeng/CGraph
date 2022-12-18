/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElementSorter.h
@Time: 2022/5/1 11:55
@Desc: 
***************************/

#ifndef CGRAPH_GELEMENTSORTER_H
#define CGRAPH_GELEMENTSORTER_H

#include <set>

#include "GElement.h"

CGRAPH_NAMESPACE_BEGIN

class GElementSorter : public GElementObject {
public:
    /**
     * 当level不同的时候，level值小的element优先被init和destroy
     * 默认情况下， init和destroy顺序随机
     * @param a
     * @param b
     * @return
     */
    CBool operator()(GElementPtr a, GElementPtr b) const {
        return (a->level_ == b->level_) ? (a < b) : (a->level_ < b->level_);
    }

    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }
};

using GSortedGElementPtrSet = std::set<GElementPtr, GElementSorter>;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GELEMENTSORTER_H
