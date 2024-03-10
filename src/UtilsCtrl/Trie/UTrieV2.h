/***************************
@Author: jianengxi
@Contact: jianengxi@gmail.com
@File: UTrieV2.h
@Time: 2024/3/9 20:35 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTRIEV2_H
#define CGRAPH_UTRIEV2_H

#include "../UtilsObject.h"
#include "UTrieNode.h"

CGRAPH_NAMESPACE_BEGIN

class UTrieV2 : public UtilsObject {
public:
    explicit UTrieV2();
    ~UTrieV2() override;

    /**
     * 插入路径信息
     * @param path
     */
    void insert(const std::string& path);

    /**
     * 查询路径信息
     * @param path
     * @return
     */
    bool find(const std::string& path);

    /**
     * 清空全部路径信息
     */
    void clear();

    /**
     * 删除路径信息
     * @param path
     */
    void eraser(const std::string& path);

private:
    UTrieNodePtr head_ { nullptr };
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTRIEV2_H
