
# 本cmake文件，供三方引入CGraph引用，用于屏蔽系统和C++版本的区别

# 根据当前 CGraph-env-include.cmake 文件的位置，定位CGraph整体工程的路径
# 从而解决并兼容了直接引入和三方库引入的路径不匹配问题
get_filename_component(CGRAPH_PROJECT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)
set(CGRAPH_PROJECT_ROOT_DIR "${CGRAPH_PROJECT_CMAKE_DIR}/../")
file(GLOB_RECURSE CGRAPH_PROJECT_SRC_LIST "${CGRAPH_PROJECT_ROOT_DIR}/src/*.cpp")

IF(APPLE)
    # 非mac平台，暂时不支持自动生成session信息
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -O2 \
        -finline-functions -Wno-deprecated-declarations -Wno-c++17-extensions")
    add_definitions(-D_ENABLE_LIKELY_)
ELSEIF(UNIX)
    # linux平台，加入多线程内容
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread -Wno-format-overflow")
    add_definitions(-D_ENABLE_LIKELY_)
ELSEIF(WIN32)
    IF(MSVC)
        # windows平台，加入utf-8设置。否则无法通过编译
        # 直接Download ZIP文件，导致无法编译通过问题的解决方法，参考：https://github.com/ChunelFeng/CGraph/issues/12
        add_definitions(/utf-8)

        # 禁止几处warning级别提示
        add_compile_options(/wd4996)
        add_compile_options(/wd4267)
        add_compile_options(/wd4018)
    ENDIF()
    # 本工程也支持在windows平台上的mingw环境使用
ENDIF()

include_directories(${CGRAPH_PROJECT_ROOT_DIR}/src/)    # 直接加入"CGraph.h"文件对应的位置

# 以下三选一，本地编译执行，推荐OBJECT方式
add_library(CGraph OBJECT ${CGRAPH_PROJECT_SRC_LIST})      # 通过代码编译
# add_library(CGraph SHARED ${CGRAPH_PROJECT_SRC_LIST})    # 编译libCGraph动态库
# add_library(CGraph STATIC ${CGRAPH_PROJECT_SRC_LIST})    # 编译libCGraph静态库
