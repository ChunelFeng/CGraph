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

#include "GOptimzerObject.h"

CGRAPH_NAMESPACE_BEGIN

class GMaxParaOptimizer : public GOptimizerObject {
protected:
    CBool match(const GSortedGElementPtrSet& elements) override {
        // 如果有一个是组的话，则不进行这一项优化
        return !std::any_of(elements.begin(), elements.end(), [](GElementPtr ptr) {
            return ptr->isGroup();
        });
    }

    /**
     * 计算当前dag的最大并发度
     * @param elements
     * @return
     */
    CSize getMaxParaSize(const GSortedGElementPtrSet& elements) {
        /**
         * 计算思路如下：
         * 1. 列出来所有的可行路径(paths)
         * 2. 根据路径，生成一张全连通图，然后将图取反，得到对应的补图(reGraph)
         * 3. 计算补图的最大团中元素个数(maxCliqueSize)，即为当前dag的最大并行度
         */
        const CSize size = elements.size();
        std::vector<std::vector<GElementPtr>> paths;
        collectPaths(elements, paths);    // 根据传入的elements 的关系，分析出所有完整路径信息

        std::vector<std::vector<int>> reGraph(size, std::vector<int>(size, 1));
        buildReverseGraph(elements, paths, reGraph);    // 根据路径信息，求出全连接图的补图

        CSize maxCliqueSize = calcMaxCliqueSize(reGraph);    // 计算最大团信息
        return maxCliqueSize;
    }

    /**
     * 记录所有的path信息
     * @param paths
     * @return
     */
    CVoid collectPaths(const GSortedGElementPtrSet& elements,
                       std::vector<std::vector<GElementPtr>>& paths) {
        for (auto& ele : elements) {
            std::vector<GElementPtr> curPath;
            if (ele->dependence_.empty()) {
                collect(ele, curPath, paths);
            }
        }
    }

    /**
     * 记录 path 信息
     * @param element
     * @param curPath
     * @param paths
     * @return
     */
    CVoid collect(GElementPtr element,
                  std::vector<GElementPtr>& curPath,
                  std::vector<std::vector<GElementPtr>>& paths) {
        curPath.emplace_back(element);
        if (element->run_before_.empty()) {
            // 如果是最后一个信息了，则记录下来
            paths.emplace_back(curPath);
        } else {
            for (auto& cur : element->run_before_) {
                collect(cur, curPath, paths);
            }
        }
        curPath.pop_back();
    }

    /**
     * 基于原有dag，计算出所有链路的全连接的补图，记作graph
     * @param elements
     * @param paths
     * @param graph
     * @return
     */
    static CVoid buildReverseGraph(const GSortedGElementPtrSet& elements,
                                   const std::vector<std::vector<GElementPtr>>& paths,
                                   std::vector<std::vector<int>>& graph) {
        for (auto& path : paths) {
            for (int i = 0; i < path.size() - 1; i++) {
                // 这里的 find是一定能找到的。因为path的数据，是从elements中记录的
                int height = (int)std::distance(elements.begin(), elements.find(path[i]));
                for (int j = i + 1; j < path.size(); j++) {
                    int column = (int)std::distance(elements.begin(), elements.find(path[j]));
                    graph[height][column] = 0;
                    graph[column][height] = 0;    // 因为需要记录的是补图，所以将默认值设置为1，符合条件的设置为0
                }
            }
        }
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

        std::function<void(CSize, CSize)> backtrack = [&](CSize start, CSize depth) {
            for (CSize i = start; i < eleSize; i++) {
                if (depth + eleSize - i <= maxCliqueSize) { return; }    // 剪枝策略：剩余的元素数量，已经不足以超过 max 值了

                if (std::all_of(curClique.begin(), curClique.end(), [&](const int j) {
                    // 如果跟当前团的node，均为连接状态，则标记为可以进入当前团中
                    return 1 == graph[j][i];
                })) {
                    curClique.push_back(i);
                    backtrack(i + 1, depth + 1);    // depth 表示，团里已有元素的个数
                    curClique.pop_back();
                }
            }

            maxCliqueSize = std::max(curClique.size(), maxCliqueSize);
        };

        backtrack(0, 0);
        return maxCliqueSize;
    }

    friend class GElementManager;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMAXPARAOPTIMIZER_H
