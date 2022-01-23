/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GFunctionDefine.h
@Time: 2022/1/23 1:17 下午
@Desc: 
***************************/

#ifndef CGRAPH_GFUNCTIONDEFINE_H
#define CGRAPH_GFUNCTIONDEFINE_H

CGRAPH_NAMESPACE_BEGIN

enum class GFunctionType {
    INIT = 1,              /** 初始化函数 */
    RUN = 2,               /** 执行函数 */
    DEINIT = 3             /** 逆初始化函数 */
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GFUNCTIONDEFINE_H
