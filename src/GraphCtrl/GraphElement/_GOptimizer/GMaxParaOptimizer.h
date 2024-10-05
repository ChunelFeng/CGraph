/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMaxParaOptimizer.h
@Time: 2023/5/12 23:58
@Desc: 
***************************/

#ifndef CGRAPH_GMAXPARAOPTIMIZER_H
#define CGRAPH_GMAXPARAOPTIMIZER_H

#include <algorithm>
#include <vector>

#include "GOptimizer.h"

CGRAPH_NAMESPACE_BEGIN

class GMaxParaOptimizer : public GOptimizer {
protected:
    /**
     * 判定是否match计算条件
     * @param elements
     * @return
     */
    static CBool match(const GSortedGElementPtrSet& elements) {
        return !std::any_of(elements.begin(), elements.end(), [](GElementCPtr ptr) {
            return ptr->isGGroup();
        });
    }

    /**
     * 计算当前dag的最大并发度
     * @param elements
     * @return
     */
    static CSize getMaxParaSize(const GSortedGElementPtrSet& elements) {
        /**
         * 计算思路如下：
         * 1. 列出来所有的可行路径(paths)
         * 2. 根据路径，生成一张全连通图，然后将图取反，得到对应的补图(reGraph)
         * 3. 计算补图的最大团中元素个数(maxCliqueSize)，即为当前dag的最大并行度
         */
        const auto& paths = collectPaths(elements);
        const auto& reGraph = buildGraph(elements, paths, 0, 0, 1);
        CSize maxCliqueSize = calcMaxCliqueSize(reGraph);
        return maxCliqueSize;
    }

    /**
     * 计算当前最大团的大小
     * @param graph
     * @return
     */
    static CSize calcMaxCliqueSize(const std::vector<std::vector<int>>& graph) {
        CSize eleSize = graph.size();
        CSize maxCliqueSize = 0;    // 最大团size
        std::vector<CSize> curClique;    // 当前团

        std::function<void(CSize, CSize)> backtrace = [&](CSize start, CSize curCliqueSize) {
            for (CSize i = start; i < eleSize; i++) {
                if (curCliqueSize + eleSize - i <= maxCliqueSize) { return; }    // 剪枝策略：剩余的元素数量，已经不足以超过 max 值了

                if (std::all_of(curClique.begin(), curClique.end(), [&](const int j) {
                    // 如果跟当前团的node，均为连接状态，则标记为可以进入当前团中
                    return 1 == graph[j][i];
                })) {
                    curClique.push_back(i);
                    backtrace(i + 1, curCliqueSize + 1);    // depth 表示，团里已有元素的个数
                    curClique.pop_back();
                }
            }

            maxCliqueSize = std::max(curClique.size(), maxCliqueSize);
        };

        backtrace(0, 0);
        return maxCliqueSize;
    }

    friend class GElementManager;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMAXPARAOPTIMIZER_H
