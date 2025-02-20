/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GSeparateOptimizer.h
@Time: 2024/3/28 22:08
@Desc: 
***************************/

#ifndef CGRAPH_GSEPARATEOPTIMIZER_H
#define CGRAPH_GSEPARATEOPTIMIZER_H

#include <algorithm>

#include "GOptimizer.h"

CGRAPH_NAMESPACE_BEGIN

class GSeparateOptimizer : public GOptimizer {
protected:
    /**
     * 判断两个 element 是否是独立运行的
     * @param elements
     * @param fst
     * @param snd
     * @return
     */
    static CBool checkSeparate(const GSortedGElementPtrSet& elements,
                               GElementCPtr fst, GElementCPtr snd) {
        CBool result = false;
        const auto& paths = GOptimizer::collectPaths(elements);
        for (const auto& path : paths) {
            if (std::find(path.begin(), path.end(), fst) != path.end()
                && std::find(path.begin(), path.end(), snd) != path.end()) {
                // 如果 fst 和 snd 在同一条 path中，则表示不可能同时运行
                result = true;
                break;
            }
        }
        return result;
    }

    /**
     * 获取最近的从属节点信息
     * @param fst
     * @param snd
     * @param fstPatch
     * @param sndPatch
     * @return
     * @notice 如果没有最近从属节点，即不在同一个 group中，则返回 nullptr
     */
    static GElementPtr getNearestAncestor(GElementCPtr fst, GElementCPtr snd,
                                          GElementPPtr fstPatch, GElementPPtr sndPatch) {
        GElementPtr ancestor = nullptr;
        const auto& fstPath = fst->getDeepPath(false);
        const auto& sndPath = snd->getDeepPath(false);
        CSize minSize = std::min(fstPath.size(), sndPath.size());
        for (CSize i = 0; i < minSize && (fstPath[i] == sndPath[i]); i++) {
            ancestor = fstPath[i];    // 最后一个一致的
        }

        (*fstPatch) = getPatch(fstPath, ancestor, fst);
        (*sndPatch) = getPatch(sndPath, ancestor, snd);
        return ancestor;
    }

    /**
     * 根据 ancestor信息，获取对等同级的element信息
     * @param path
     * @param ancestor
     * @param element
     * @return
     */
    static GElementPtr getPatch(const GElementPtrArr& path, GElementCPtr ancestor, GElementCPtr element) {
        GElementPtr patch = nullptr;
        for (int i = (int)path.size() - 1; i >= 0 && (ancestor != element); i--) {
            if (path[i] == ancestor) {
                break;
            }
            patch = path[i];
        }
        return patch;
    }

    friend class GPipeline;
    friend class GRegion;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSEPARATEOPTIMIZER_H
