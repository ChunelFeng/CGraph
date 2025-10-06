/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStorage.h
@Time: 2025/10/4 13:54
@Desc: support cpp17 and upper only
***************************/

#ifndef CGRAPH_GSTORAGE_H
#define CGRAPH_GSTORAGE_H

#if __cplusplus >= 201703L

#include "../../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

class GPipeline;

class GStorage : public GraphObject {
protected:
    explicit GStorage() = default;

    /**
     * 保存
     * @param pipeline
     * @param path
     * @return
     */
    static CStatus save(GPipeline* pipeline, const std::string& path);

    /**
     * 加载
     * @param pipeline
     * @param path
     * @return
     */
    static CStatus load(GPipeline* pipeline, const std::string& path);

    CGRAPH_NO_ALLOWED_COPY(GStorage)

private:
    /**
     * 从 buffer 中加载并还原pipeline 数据信息
     * @param pipeline
     * @param buffer
     * @param size
     * @return
     */
    static CStatus loadBuffer(GPipeline* pipeline, char* buffer, CSize size);

    friend class GPipeline;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTORAGE_H

#endif