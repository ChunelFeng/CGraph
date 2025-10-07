/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GStorageFactory.cpp
@Time: 2025/10/4 22:09
@Desc: 
***************************/

#include "GStorageFactory.h"

CGRAPH_NAMESPACE_BEGIN

std::map<std::string, std::function<GraphObject*()>> GStorageFactory::meta_types_ {};

GraphObject* GStorageFactory::createByType(const std::string& typeName) {
    const auto& it = meta_types_.find(typeName);
    return (it == meta_types_.end()) ? nullptr : it->second();
}

CGRAPH_NAMESPACE_END

