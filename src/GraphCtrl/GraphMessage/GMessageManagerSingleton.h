/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GMessageManagerSingleton.h
@Time: 2023/9/23 00:47
@Desc:
***************************/

#ifndef CGRAPH_GMESSAGEMANAGERSINGLETON_H
#define CGRAPH_GMESSAGEMANAGERSINGLETON_H

#include "GMessageObject.h"
#include "GMessageManager.h"

CGRAPH_NAMESPACE_BEGIN

class GMessageManagerSingleton : public GMessageObject {
public:
    /**
     * 获取 GMessageManager单例的句柄信息
     * @return
     */
    static GMessageManager<>* get() {
        return GMessageManagerSingleton::singleton_.get();
    }

private:
    static USingleton<GMessageManager<>, USingletonType::LAZY, false> singleton_;    // message管理类。注意：这个单例，是不需要被init()的
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GMESSAGEMANAGERSINGLETON_H
