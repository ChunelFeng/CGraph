/***************************
@Author: YeShenYong
@Contact: 1050575224@qq.com
@File: GEngineDefine.h
@Time: 2022/12/18 19:59 下午
@Desc: 
***************************/

#ifndef CGRAPH_GENGINEDEFINE_H
#define CGRAPH_GENGINEDEFINE_H

CGRAPH_NAMESPACE_BEGIN

enum class GEngineType {
    STATIC = 0,             /** 静态图运行 */
    DYNAMIC = 1,            /** 动态图运行 */
    TOPO = 2,               /** 拓扑运行 */
};

CGRAPH_INTERNAL_NAMESPACE_BEGIN
enum class GEngineDagType {
    COMMON = 0,                /** 传统的dag类型 */
    ALL_SERIAL = 1,            /** 所有的element都是串行的 */
    ALL_PARALLEL = 2,          /** 所有的element都是并行的 */
};
CGRAPH_INTERNAL_NAMESPACE_END

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GENGINEDEFINE_H