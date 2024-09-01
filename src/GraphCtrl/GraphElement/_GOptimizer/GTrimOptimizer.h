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
            int idx = (int)std::distance(elements.begin(), elements.find(cur));
            GElementPtrArr candidates;
            for (int i = 0; i < (int)cur->dependence_.size(); i++) {
                int x = (int)std::distance(elements.begin(), elements.find(cur->dependence_[i]));
                for (int j = i; j < (int)cur->dependence_.size(); j++) {
                    int y = (int)std::distance(elements.begin(), elements.find(cur->dependence_[j]));
                    if (1 == graph[x][y]) {
                        graph[x][idx] = 0;
                        candidates.push_back(cur->dependence_[i]);
                    }
                }
            }

            for (auto* candidate : candidates) {
                if (cur->removeDepend(candidate)) {
                    trimNum++;
                }
            }
        }

        return trimNum;
    }

    friend class GElementManager;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GTRIMOPTIMIZER_H
