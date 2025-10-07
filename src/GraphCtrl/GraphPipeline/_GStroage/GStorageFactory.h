/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStorageFactory.h
@Time: 2025/10/4 21:44
@Desc: 
***************************/

#ifndef CGRAPH_GSTORAGEFACTORY_H
#define CGRAPH_GSTORAGEFACTORY_H

#include <map>

#include "../../GraphObject.h"

CGRAPH_NAMESPACE_BEGIN

class GElement;
class GEvent;
class GParam;
class GDaemon;
class GStage;
class GAspect;

class GStorageFactory : public GraphObject {
public:
    /**
     * 注册原始类型信息
     * @tparam T
     * @return
     */
    template<typename T,
            c_enable_if_t<std::is_base_of<GElement, T>::value
                         || std::is_base_of<GParam, T>::value
                         || std::is_base_of<GDaemon, T>::value
                         || std::is_base_of<GAspect, T>::value
                         || std::is_base_of<GStage, T>::value
                         || std::is_base_of<GEvent, T>::value, int> = 0>
    static CVoid registerMetaType() {
        const char* key = typeid(T).name();
        meta_types_[key] = []() { return CAllocator::safeMallocCObject<T>(); };
    }

protected:
    /**
     * 通过 type 信息创建节点
     * @param typeName
     * @return
     */
    static GraphObject* createByType(const std::string& typeName);

    explicit GStorageFactory() = default;
    ~GStorageFactory() override = default;

private:
    static std::map<std::string, std::function<GraphObject*()> > meta_types_;

    friend class GStorage;
};

#define CGRAPH_REGISTER_META_TYPE(_CLZ) GStorageFactory::registerMetaType<_CLZ>();    \

CGRAPH_NAMESPACE_END

#endif //CGRAPH_GSTORAGEFACTORY_H
