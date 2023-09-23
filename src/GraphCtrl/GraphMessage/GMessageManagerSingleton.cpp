/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageManagerSingleton.cpp
@Time: 2023/9/23 01:06
@Desc: 
***************************/

#include "GMessageManagerSingleton.h"

CGRAPH_NAMESPACE_BEGIN

/**
 * GMessageManager单例的句柄信息
 * message管理类，必须在这里实现。
 * 如果是放在 GMessageManager.h 文件中，以外部申明static变量的形式存在
 * 在cpp文件中引用的时候，会出现被多次构造的情况
 * 参考：https://github.com/ChunelFeng/CGraph/issues/221
 */
USingleton<GMessageManager<>, USingletonType::LAZY, false> GMessageManagerSingleton::singleton_;

CGRAPH_NAMESPACE_END
