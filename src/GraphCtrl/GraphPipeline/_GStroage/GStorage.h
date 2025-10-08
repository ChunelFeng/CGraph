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

#include "GStorageDefine.h"
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
     * 构建 pipeline storage
     * @param pipeline
     * @param storage
     * @return
     */
    static CStatus buildPipelineStorage(GPipeline* pipeline, _GPipelineStorage& storage);

    /**
     * 将 storage 存储到 buffer 中
     * @param storage
     * @param path
     * @return
     */
    static CStatus saveBuffer(const _GPipelineStorage& storage, const std::string& path);

    /**
     * 从 buffer 中加载并还原pipeline 数据信息
     * @param pipeline
     * @param buffer
     * @param size
     * @return
     */
    static CStatus loadBuffer(GPipeline* pipeline, char* buffer, CSize size);
    static CStatus loadElement(GPipeline* pipeline, const _GPipelineStorage& storage);
    static CStatus loadEvent(GPipeline* pipeline, const _GPipelineStorage& storage);
    static CStatus loadParam(GPipeline* pipeline, const _GPipelineStorage& storage);
    static CStatus loadDaemon(GPipeline* pipeline, const _GPipelineStorage& storage);
    static CStatus loadStage(GPipeline* pipeline, const _GPipelineStorage& storage);

    static CStatus loadAspect(GElementPtr element, const std::vector<_GAspectStorage>& aspStorages);

    friend class GPipeline;
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTORAGE_H

#endif