/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UTrie.h
@Time: 2021/9/19 4:22 下午
@Desc: 
***************************/

#ifndef CGRAPH_UTRIE_H
#define CGRAPH_UTRIE_H

#include "../UtilsObject.h"
#include "UTrieNode.h"

CGRAPH_NAMESPACE_BEGIN

class UTrie : public UtilsObject {
public:
    explicit UTrie();
    ~UTrie() override;

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

protected:
    bool innerFind(UTrieNodePtr node, const std::string& path, int index);
    void innerInsert(UTrieNodePtr node, const std::string& path, int index);
    void innerClear(UTrieNodePtr node);
    void innerEraser(UTrieNodePtr node, const std::string& path, int index, bool& isErased);

private:
    UTrieNodePtr head_ { nullptr };
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UTRIE_H
