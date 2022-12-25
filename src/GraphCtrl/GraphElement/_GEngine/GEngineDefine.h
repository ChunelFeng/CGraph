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
    STATIC = 1,             /** 静态图运行 */
    DYNAMIC = 2             /** 动态图运行 */
};

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GENGINEDEFINE_H