/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: CDescInfo.h
@Time: 2023/2/19 15:56
@Desc: 通用描述信息
***************************/

#ifndef CGRAPH_CDESCINFO_H
#define CGRAPH_CDESCINFO_H

#include <string>

#include "CBasicDefine.h"

CGRAPH_NAMESPACE_BEGIN

class CDescInfo {
public:
    /**
     * 获取名称信息
     * @return
     */
    const std::string& getName() const {
        return name_;
    }

    /**
     * 获取唯一id信息
     * @return
     */
    const std::string& getSession() const {
        return session_;
    }

    /**
     * 获取描述信息
     * @return
     */
    const std::string& getDescription() const {
        return description_;
    }

    /**
     * 设置名称信息
     * @param name
     * @return
     */
    virtual auto setName(const std::string& name)
    -> decltype(this) {
        name_ = name;
        return this;
    }

    /**
     * 设置描述信息
     * @param description
     * @return
     */
    virtual auto setDescription(const std::string& description)
    -> decltype(this) {
        description_ = description;
        return this;
    }

    virtual ~CDescInfo() = default;

protected:
    std::string name_;                               // 名字
    std::string session_;                            // 唯一id信息
    std::string description_;                        // 描述信息
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_CDESCINFO_H
