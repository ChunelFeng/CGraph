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

UREFL_CREATE_STRUCT_TRAIT_INFO(_GEventStorage,
    UREFL_DECLARE_FIELD(_GEventStorage, key_, 1),
    UREFL_DECLARE_FIELD(_GEventStorage, clz_name_, 2)
)

UREFL_CREATE_STRUCT_TRAIT_INFO(_GAspectStorage,
    UREFL_DECLARE_FIELD(_GAspectStorage, clz_name_, 1)
)

UREFL_CREATE_STRUCT_TRAIT_INFO(_GDaemonStorage,
    UREFL_DECLARE_FIELD(_GDaemonStorage, msec_, 1),
    UREFL_DECLARE_FIELD(_GDaemonStorage, clz_name_, 2)
)

UREFL_CREATE_STRUCT_TRAIT_INFO(_GStageStorage,
    UREFL_DECLARE_FIELD(_GStageStorage, key_, 1),
    UREFL_DECLARE_FIELD(_GStageStorage, threshold_, 2),
    UREFL_DECLARE_FIELD(_GStageStorage, clz_name_, 3)
)

UREFL_CREATE_STRUCT_TRAIT_INFO(_GParamStorage,
    UREFL_DECLARE_FIELD(_GParamStorage, key_, 1),
    UREFL_DECLARE_FIELD(_GParamStorage, clz_name_, 2)
)

UREFL_CREATE_STRUCT_TRAIT_INFO(_GElementStorage,
    UREFL_DECLARE_FIELD(_GElementStorage, name_, 1),
    UREFL_DECLARE_FIELD(_GElementStorage, loop_, 2),
    UREFL_DECLARE_FIELD(_GElementStorage, session_, 3),
    UREFL_DECLARE_FIELD(_GElementStorage, dependence_sessions_, 4),
    UREFL_DECLARE_FIELD(_GElementStorage, visible_, 5),
    UREFL_DECLARE_FIELD(_GElementStorage, binding_index_, 6),
    UREFL_DECLARE_FIELD(_GElementStorage, level_, 7),
    UREFL_DECLARE_FIELD(_GElementStorage, timeout_, 8),
    UREFL_DECLARE_FIELD(_GElementStorage, timeout_strategy_, 9),
    UREFL_DECLARE_FIELD(_GElementStorage, is_marco_, 10),
    UREFL_DECLARE_FIELD(_GElementStorage, clz_name_, 11),
    UREFL_DECLARE_FIELD(_GElementStorage, belong_session_, 12),
    UREFL_DECLARE_FIELD(_GElementStorage, element_type_, 13),
    UREFL_DECLARE_FIELD(_GElementStorage, children_, 14),
    UREFL_DECLARE_FIELD(_GElementStorage, aspect_storages_, 15)
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
    UREFL_DECLARE_FIELD(_GPipelineStorage, event_storages_, 2),
    UREFL_DECLARE_FIELD(_GPipelineStorage, param_storages_, 3),
    UREFL_DECLARE_FIELD(_GPipelineStorage, daemon_storages_, 4),
    UREFL_DECLARE_FIELD(_GPipelineStorage, stage_storages_, 5),
    UREFL_DECLARE_FIELD(_GPipelineStorage, thread_pool_config_, 6)
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
    CGRAPH_ASSERT_NOT_NULL(pipeline)

    _GPipelineStorage storage {};
    status += buildPipelineStorage(pipeline, storage);
    CGRAPH_FUNCTION_CHECK_STATUS

    status += saveBuffer(storage, path);
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


CStatus GStorage::buildPipelineStorage(GPipeline* pipeline, _GPipelineStorage& storage) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline,
                           pipeline->event_manager_,
                           pipeline->param_manager_,
                           pipeline->daemon_manager_,
                           pipeline->stage_manager_);

    for (const auto* cur : pipeline->repository_.elements_) {
        storage.element_storages_.emplace_back(cur);
    }
    for (const auto& event : pipeline->event_manager_->events_map_) {
        const std::string& key = event.first;
        const std::string& clz = typeid(*event.second).name();
        storage.event_storages_.emplace_back(key, clz);
    }
    for (const auto& param : pipeline->param_manager_->params_map_) {
        const std::string& key = param.first;
        const std::string& clz = typeid(*param.second).name();
        bool backtrace = (*param.second).backtrace_enable_;
        storage.param_storages_.emplace_back(key, backtrace, clz);
    }
    for (const auto& daemon : pipeline->daemon_manager_->daemons_) {
        CMSec msec = daemon->getInterval();
        const std::string& clz = typeid(*daemon).name();
        storage.daemon_storages_.emplace_back(msec, clz);
    }
    for (const auto& stage : pipeline->stage_manager_->stage_map_) {
        const std::string& key = stage.first;
        CInt threshold = stage.second->threshold_;
        const std::string& clz = typeid(*stage.second).name();
        storage.stage_storages_.emplace_back(key, threshold, clz);
    }
    storage.thread_pool_config_ = pipeline->schedule_.config_;
    CGRAPH_FUNCTION_END
}


CStatus GStorage::saveBuffer(const _GPipelineStorage& storage, const std::string& path) {
    CGRAPH_FUNCTION_BEGIN

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


CStatus GStorage::loadBuffer(GPipelinePtr pipeline, char* buffer, CSize size) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline, buffer)

    _GPipelineStorage storage {};
    auto refl = internal::UReflection<_GPipelineStorage>();
    refl.read(storage, (CUCharPtr)buffer, size);

    status += loadElement(pipeline, storage);    // element 中间包含校验。如果校验失败，立刻退出即可
    CGRAPH_FUNCTION_CHECK_STATUS

    status += loadEvent(pipeline, storage);
    status += loadParam(pipeline, storage);
    status += loadDaemon(pipeline, storage);
    status += loadStage(pipeline, storage);
    CGRAPH_FUNCTION_CHECK_STATUS

    pipeline->setUniqueThreadPoolConfig(storage.thread_pool_config_);

    CGRAPH_FUNCTION_END
}


CStatus GStorage::loadElement(GPipelinePtr pipeline, const _GPipelineStorage& storage) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline)

    std::map<std::string, GElementPtr> eleCache{};    // 用于存放所有的构建好的 element 信息
    // 恢复所有的 element 的信息
    for (const auto& cur : storage.element_storages_) {
        auto element = dynamic_cast<GElementPtr>(GStorageFactory::createByType(cur.clz_name_));
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!element,
                                                cur.name_ + " element do not register, please check")
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(element->element_type_ != cur.element_type_,
                                                cur.name_ + " check type failed when load");
        status = cur.recover(element);
        CGRAPH_FUNCTION_CHECK_STATUS

        eleCache[cur.session_] = element;
    }

    // 设定从属关系
    for (const auto& cur : storage.element_storages_) {
        auto element = eleCache[cur.session_];
        if (cur.belong_session_.empty()) {
            pipeline->innerRegister(element, {}, cur.name_, cur.loop_);
        } else {
            element->belong_ = eleCache[cur.belong_session_];
        }

        status += loadAspect(element, cur.aspect_storages_);
        CGRAPH_FUNCTION_CHECK_STATUS
    }

    // 设定依赖关系
    for (const auto& cur : storage.element_storages_) {
        if (!cur.dependence_sessions_.empty()) {
            auto element = eleCache[cur.session_];
            GElementPtrSet depSet{};
            for (const std::string& dep : cur.dependence_sessions_) {
                depSet.insert(eleCache[dep]);
            }
            status += element->addDependGElements(depSet);
            CGRAPH_FUNCTION_CHECK_STATUS
        }

        if (!cur.children_.empty()) {
            GGroupPtr group = dynamic_cast<GGroupPtr>(eleCache[cur.session_]);
            CGRAPH_ASSERT_NOT_NULL(group)
            for (const auto& child : cur.children_) {
                group->addElement(eleCache[child.session_]);
            }
        }
    }

    CGRAPH_FUNCTION_END
}


CStatus GStorage::loadEvent(GPipelinePtr pipeline, const _GPipelineStorage& storage) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline, pipeline->event_manager_, pipeline->param_manager_)

    // 为了确保每个 event 中都可以获取参数信息
    pipeline->event_manager_->param_manager_ = pipeline->param_manager_;
    for (const auto& ges : storage.event_storages_) {
        GEventPtr event = dynamic_cast<GEventPtr>(GStorageFactory::createByType(ges.clz_name_));
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!event,
                                                ges.key_ + " event type do not register, please check");
        pipeline->event_manager_->events_map_[ges.key_] = event;
    }

    CGRAPH_FUNCTION_END
}


CStatus GStorage::loadParam(GPipelinePtr pipeline, const _GPipelineStorage& storage) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline, pipeline->param_manager_)

    for (const auto& gps : storage.param_storages_) {
        GParamPtr param = dynamic_cast<GParamPtr>(GStorageFactory::createByType(gps.clz_name_));
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!param,
                                                gps.key_ + " param type do not register, please check");
        param->key_ = gps.key_;
        param->backtrace_enable_ = gps.backtrace_;
        pipeline->param_manager_->params_map_[gps.key_] = param;
    }

    CGRAPH_FUNCTION_END
}


CStatus GStorage::loadDaemon(GPipelinePtr pipeline, const _GPipelineStorage& storage) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline,
                           pipeline->daemon_manager_,
                           pipeline->event_manager_,
                           pipeline->param_manager_)

    for (const auto& gds : storage.daemon_storages_) {
        GDaemonPtr daemon = dynamic_cast<GDaemonPtr>(GStorageFactory::createByType(gds.clz_name_));
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!daemon,
                                                gds.clz_name_ + " daemon type do not register, please check");
        daemon->setInterval(gds.msec_);
        daemon->setGEventManager(pipeline->event_manager_);
        daemon->setGParamManager(pipeline->param_manager_);
        pipeline->daemon_manager_->daemons_.insert(daemon);
    }

    CGRAPH_FUNCTION_END
}


CStatus GStorage::loadStage(GPipelinePtr pipeline, const _GPipelineStorage& storage) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(pipeline,
                           pipeline->stage_manager_,
                           pipeline->event_manager_,
                           pipeline->param_manager_);

    for (const auto& gss : storage.stage_storages_) {
        GStagePtr stage = dynamic_cast<GStagePtr>(GStorageFactory::createByType(gss.clz_name_));
        CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!stage,
                                                gss.key_ + " stage type do not register, please check");
        stage->threshold_ = gss.threshold_;
        stage->param_manager_ = pipeline->param_manager_;
        pipeline->stage_manager_->stage_map_[gss.key_] = stage;
    }

    CGRAPH_FUNCTION_END
}


CStatus GStorage::loadAspect(GElementPtr element, const std::vector<_GAspectStorage>& aspStorages) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_NOT_NULL(element)

    if (!aspStorages.empty()) {
        element->aspect_manager_ = CAllocator::safeMallocCObject<GAspectManager>();
        for (const auto &gas: aspStorages) {
            GAspectPtr aspect = dynamic_cast<GAspectPtr>(GStorageFactory::createByType(gas.clz_name_));
            CGRAPH_RETURN_ERROR_STATUS_BY_CONDITION(!aspect,
                                                    gas.clz_name_ + " type create aspect failed.");
            element->aspect_manager_->add(aspect);
        }
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END

#endif
