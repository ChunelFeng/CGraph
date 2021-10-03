/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GAspectObject.h
@Time: 2021/10/2 9:46 下午
@Desc: 
***************************/

#ifndef CGRAPH_GASPECTOBJECT_H
#define CGRAPH_GASPECTOBJECT_H

#include <string>

#include "../GraphObject.h"
#include "GAspectDefine.h"

class GAspectObject : public GraphObject {
public:
    /**
     * 获取name信息
     * @return
     */
    virtual const std::string& getName() final {
        return name_;
    }

    /**
     * 设置名称
     * @param name
     */
    virtual void setName(const std::string& name) {
        this->name_ = name;
    }

protected:
    /**
     * GAspect 相关内容，不需要执行run方法
     * @return
     */
    CSTATUS run() final {
        CGRAPH_NO_SUPPORT;
    }

private:
    std::string name_;            // 切面类名称，跟element名称保持相同
};


#endif //CGRAPH_GASPECTOBJECT_H
