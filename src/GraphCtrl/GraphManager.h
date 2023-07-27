/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GraphManager.h
@Time: 2022/2/1 5:18 下午
@Desc: 
***************************/

#ifndef CGRAPH_GRAPHMANAGER_H
#define CGRAPH_GRAPHMANAGER_H


#include <string>

#include "../CBasic/CBasicInclude.h"
#include "GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T,
        c_enable_if_t<std::is_base_of<GraphObject, T>::value, int> = 0>
class GraphManager : public CObject {
protected:
    /**
     * 添加一个object
     * @param object
     * @return
     */
    virtual CStatus add(T* object) {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 删除一个object
     * @param object
     * @return
     */
    virtual CStatus remove(T* object) {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 查找一个object
     * @param object
     * @return
     */
    virtual CBool find(T* object) const {
        return false;
    }

    /**
     * 创建一个object
     * @param key
     * @return
     */
    virtual CStatus create(const std::string& key) {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 获取一个object
     * @param key
     * @return
     */
    virtual T* get(const std::string& key) {
        return nullptr;
    }

    /**
     * 重置所有object
     * @return
     */
    virtual CStatus reset() {
        CGRAPH_NO_SUPPORT
    }

    /**
     * 获取大小信息
     * @return
     */
    virtual CSize getSize() const {
        return 0;
    }

    /**
     * 清空所有object，必须实现的内容
     * @return
     */
    virtual CStatus clear() = 0;

    /**
     * 执行函数，默认为执行失败
     * @return
     */
    CStatus run() override {
        CGRAPH_NO_SUPPORT
    }
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GRAPHMANAGER_H
