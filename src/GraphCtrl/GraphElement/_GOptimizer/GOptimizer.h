/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GOptimizer.h
@Time: 2024/4/7 21:27
@Desc: 
***************************/

#ifndef CGRAPH_GOPTIMIZER_H
#define CGRAPH_GOPTIMIZER_H

#include "../GElementObject.h"

CGRAPH_NAMESPACE_BEGIN

class GOptimizer : public GElementObject {
protected:
    /**
     * 记录 path 信息
     * @param element
     * @param curPath
     * @param paths
     * @return
     */
    static CVoid collect(GElementPtr element,
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
     * 记录所有的path信息
     * @param elements
     * @return
     */
    static std::vector<std::vector<GElementPtr>> collectPaths(const GSortedGElementPtrSet& elements) {
        std::vector<std::vector<GElementPtr>> paths;
        for (const auto& element : elements) {
            std::vector<GElementPtr> curPath;
            if (element->dependence_.empty()) {
                collect(element, curPath, paths);
            }
        }

        return paths;
    }

    friend class GPerf;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GOPTIMIZER_H
