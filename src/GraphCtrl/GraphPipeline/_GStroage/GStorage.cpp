/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStorage.cpp
@Time: 2025/10/4 14:01
@Desc: 
***************************/

#if __cplusplus >= 201703L

#include <fstream>

#include "GStorage.h"
#include "GStorageDefine.h"
#include "../GPipeline.h"

CGRAPH_NAMESPACE_BEGIN
CGRAPH_INTERNAL_NAMESPACE_BEGIN

UREFL_CREATE_STRUCT_TRAIT_INFO(_GElementStorage,
   UREFL_DECLARE_FIELD(_GElementStorage, name_, 1),
   UREFL_DECLARE_FIELD(_GElementStorage, loop_, 2),
   UREFL_DECLARE_FIELD(_GElementStorage, dependence_, 3),
   UREFL_DECLARE_FIELD(_GElementStorage, visible_, 4),
   UREFL_DECLARE_FIELD(_GElementStorage, binding_index_, 5),
   UREFL_DECLARE_FIELD(_GElementStorage, level_, 7),
   UREFL_DECLARE_FIELD(_GElementStorage, timeout_, 8),
   UREFL_DECLARE_FIELD(_GElementStorage, timeout_strategy_, 9),
   UREFL_DECLARE_FIELD(_GElementStorage, is_marco_, 10),
   UREFL_DECLARE_FIELD(_GElementStorage, clz_name_, 11),
   UREFL_DECLARE_FIELD(_GElementStorage, belong_name_, 12),
   UREFL_DECLARE_FIELD(_GElementStorage, element_type_, 13),
   UREFL_DECLARE_FIELD(_GElementStorage, children_, 14)
)

UREFL_CREATE_STRUCT_TRAIT_INFO(UThreadPoolConfig,
   UREFL_DECLARE_FIELD(UThreadPoolConfig, default_thread_size_, 1),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, secondary_thread_size_, 2),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, max_thread_size_, 3),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, max_task_steal_range_, 4),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, max_local_batch_size_, 5),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, max_pool_batch_size_, 6),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, max_steal_batch_size_, 7),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, primary_thread_busy_epoch_, 8),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, primary_thread_empty_interval_, 9),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, secondary_thread_ttl_, 10),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, monitor_span_, 11),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, queue_emtpy_interval_, 12),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, primary_thread_policy_, 13),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, secondary_thread_policy_, 14),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, primary_thread_priority_, 15),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, secondary_thread_priority_, 16),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, bind_cpu_enable_, 17),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, batch_task_enable_, 18),
   UREFL_DECLARE_FIELD(UThreadPoolConfig, monitor_enable_, 19)
)

UREFL_CREATE_STRUCT_TRAIT_INFO(_GPipelineStorage,
    UREFL_DECLARE_FIELD(_GPipelineStorage, element_storages_, 1),
    UREFL_DECLARE_FIELD(_GPipelineStorage, thread_pool_config_, 2)
)

CGRAPH_INTERNAL_NAMESPACE_END

/**
 * 保存当前 pipeline 信息
 * @param pipeline
 * @param path
 * @return
 */
CStatus GStorage::save(GPipelinePtr pipeline, const std::string& path) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline);

    _GPipelineStorage storage {};
    for (const auto* cur : pipeline->repository_.elements_) {
        storage.element_storages_.emplace_back(_GElementStorage(cur));
    }
    storage.thread_pool_config_ = pipeline->schedule_.config_;

    auto refl = internal::UReflection<_GPipelineStorage>();
    auto size = refl.getSize(storage);
    auto* buffer = new (std::nothrow) char [size];
    CGRAPH_ASSERT_NOT_NULL(buffer)
    memset(buffer, 0, size);
    std::ofstream ofd(path, std::ios::binary);
    do {
        if (!ofd) {
            status = CStatus("Failed write open file: " + path);
            break;
        }

        refl.write(storage, (CUCharPtr)buffer, size);
        ofd.write(buffer, (CLong)size);
        ofd.flush();

        if (!ofd) {
            status = CStatus("Failed to write file: " + path);
        }
    } while (0);

    ofd.close();
    CGRAPH_DELETE_PTR_ARRAY(buffer)

    CGRAPH_FUNCTION_END
}


/**
 * 加载当前 pipeline 信息
 * @param pipeline
 * @param path
 * @return
 */
CStatus GStorage::load(GPipelinePtr pipeline, const std::string& path) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline)
    std::ifstream ifd(path, std::ios::binary);
    if (!ifd) {
        return CStatus("Failed read open file: " + path);
    }

    ifd.seekg(0, std::ios::end);
    auto size = ifd.tellg();
    ifd.seekg(0, std::ios::beg);

    char* buffer = new (std::nothrow) char[size];
    CGRAPH_ASSERT_NOT_NULL(buffer)
    memset(buffer, 0, size);

    status = ifd.read(buffer, size)
            ? loadBuffer(pipeline, buffer, size)
            : CStatus("Failed to read file: " + path);

    ifd.close();
    CGRAPH_DELETE_PTR_ARRAY(buffer)
    CGRAPH_FUNCTION_END
}


CStatus GStorage::loadBuffer(GPipelinePtr pipeline, char* buffer, CSize size) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline, buffer)

    _GPipelineStorage storage {};
    auto refl = internal::UReflection<_GPipelineStorage>();
    refl.read(storage, (CUCharPtr)buffer, size);

    std::map<std::string, GElementPtr> eleCache{};    // 用于存放所有的构建好的 element 信息
    // 恢复所有的 element 的信息
    for (const auto& cur : storage.element_storages_) {
        auto element = dynamic_cast<GElementPtr>(GStorageFactory::createByType(cur.clz_name_.c_str()));
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!element,
                                                cur.name_ + " element type do not register, please check")
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(element->element_type_ != cur.element_type_,
                                                cur.name_ + " check type failed when load");
        status = cur.recover(element);
        CGRAPH_FUNCTION_CHECK_STATUS

        eleCache[cur.name_] = element;
    }

    // 设定从属关系
    for (const auto& cur : storage.element_storages_) {
        auto element = eleCache[cur.name_];
        if (cur.belong_name_.empty()) {
            pipeline->innerRegister(element, {}, cur.name_, cur.loop_);
        } else {
            element->belong_ = eleCache[cur.belong_name_];
        }
    }

    // 设定依赖关系
    for (const auto& cur : storage.element_storages_) {
        if (!cur.dependence_.empty()) {
            auto element = eleCache[cur.name_];
            GElementPtrSet depSet{};
            for (const std::string& dep : cur.dependence_) {
                depSet.insert(eleCache[dep]);
            }
            status += element->addDependGElements(depSet);
            CGRAPH_FUNCTION_CHECK_STATUS
        }

        if (!cur.children_.empty()) {
            GGroupPtr group = dynamic_cast<GGroupPtr>(eleCache[cur.name_]);
            CGRAPH_ASSERT_NOT_NULL(group)
            for (const auto& child : cur.children_) {
                group->addElement(eleCache[child.name_]);
            }
        }
    }
    pipeline->setUniqueThreadPoolConfig(storage.thread_pool_config_);

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif
