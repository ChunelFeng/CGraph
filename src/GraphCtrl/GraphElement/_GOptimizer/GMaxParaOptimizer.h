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
    GMaxParaOptimizer() = default;

    CBool match(const GSortedGElementPtrSet& elements) override;

    /**
     * 计算当前dag的最大并发度
     * @param elements
     * @return
     */
    CSize getMaxParaSize(const GSortedGElementPtrSet& elements);

    /**
     * 记录所有的path信息
     * @param paths
     * @return
     */
    CVoid collectPaths(const GSortedGElementPtrSet& elements,
                       std::vector<std::vector<GElementPtr>>& paths);

    /**
     * 记录 path 信息
     * @param element
     * @param curPath
     * @param paths
     * @return
     */
    CVoid collect(GElementPtr element,
                  std::vector<GElementPtr>& curPath,
                  std::vector<std::vector<GElementPtr>>& paths);

    /**
     * 基于原有dag，计算出所有链路的全连接的补图，记作graph
     * @param elements
     * @param paths
     * @param graph
     * @return
     */
    CVoid buildReverseGraph(const GSortedGElementPtrSet& elements,
                            const std::vector<std::vector<GElementPtr>>& paths,
                            std::vector<std::vector<int>>& graph);

    /**
     * 计算当前最大团的大小
     * @param graph
     * @return
     */
    CSize calcMaxCliqueSize(const std::vector<std::vector<int>>& graph);

    friend class GElementManager;
};

using GMaxParaOptimizerPtr = GMaxParaOptimizer *;

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMAXPARAOPTIMIZER_H
