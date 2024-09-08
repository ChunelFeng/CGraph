/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GTrimOptimizer.h
@Time: 2024/9/1 13:59
@Desc: 
***************************/

#ifndef CGRAPH_GTRIMOPTIMIZER_H
#define CGRAPH_GTRIMOPTIMIZER_H

#include <algorithm>
#include <iterator>

#include "GOptimizer.h"
#include "../GGroup/GGroupInclude.h"

CGRAPH_NAMESPACE_BEGIN

class GTrimOptimizer : public GOptimizer {
    /**
     * 针对图结构，做剪裁
     * @param elements
     * @return
     */
    static CSize trim(GSortedGElementPtrSet& elements) {
        CSize trimNum = 0;
        const auto& paths = collectPaths(elements);
        auto graph = buildGraph(elements, paths, 1, 0, 0);

        for (auto* cur : elements) {
            CGRAPH_ASSERT_NOT_NULL_THROW_ERROR(cur)
            GElementPtrArr candidates;
            for (CSize i = 0; i < cur->dependence_.size(); i++) {
                CSize x = std::distance(elements.begin(), elements.find(cur->dependence_[i]));
                for (CSize j = 0; j < cur->dependence_.size(); j++) {
                    // 这里必须是 n^2 的循环
                    CSize y = std::distance(elements.begin(), elements.find(cur->dependence_[j]));
                    if (1 == graph[x][y]) {
                        candidates.push_back(cur->dependence_[i]);
                    }
                }
            }

            for (auto* candidate : candidates) {
                if (cur->removeDepend(candidate).isOK()) {
                    trimNum++;
                }
            }

            if (GElementType::REGION == cur->element_type_) {
                trimNum += ((GRegionPtr)cur)->trim();
            }
        }

        return trimNum;
    }

    friend class GElementManager;
    friend class GRegion;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTRIMOPTIMIZER_H
