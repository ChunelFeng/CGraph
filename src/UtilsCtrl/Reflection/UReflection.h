/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UReflection.h
@Time: 2025/10/4 10:35
@Desc: 
***************************/

#ifndef CGRAPH_UREFLECTION_H
#define CGRAPH_UREFLECTION_H

#include "../UtilsObject.h"
#include "UReflectionDefine.h"
#include "UReflectionSession.h"
#include "UReflectionType.h"

CGRAPH_NAMESPACE_BEGIN
CGRAPH_INTERNAL_NAMESPACE_BEGIN

template<class T>
class UReflection : public UtilsObject {
public:
    /**
     * 获取数据size信息
     * @param obj
     * @return
     */
    CSize getSize(const T &obj) {
        UReflSession session;
        reflGetSize(session, obj);
        return session.getLen();
    }

    CVoid write(const T &obj, CUCharPtr data, size_t size) {
        UReflSession session(data, size);
        reflWrite(session, obj);
    }

    CVoid read(T &obj, CUCharPtr data, size_t size) {
        UReflSession session(data, size);
        reflRead(session, obj);
    }
};

CGRAPH_INTERNAL_NAMESPACE_END
CGRAPH_NAMESPACE_END

#endif //CGRAPH_UREFLECTION_H
