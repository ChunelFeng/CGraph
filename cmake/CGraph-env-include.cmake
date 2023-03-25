
# 本cmake文件，供三方引入CGraph引用，用于屏蔽系统和C++版本的区别

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
    # windows平台，加入utf-8设置。否则无法通过编译
    add_definitions(/utf-8)

    # 禁止几处warning级别提示
    add_compile_options(/wd4996)
    add_compile_options(/wd4267)
    add_compile_options(/wd4018)
ENDIF()

file(GLOB_RECURSE CGRAPH_SRC_LIST "./src/*.cpp")

# 以下三选一，本地编译执行，推荐OBJECT方式
add_library(CGraph OBJECT ${CGRAPH_SRC_LIST})      # 通过代码编译
# add_library(CGraph SHARED ${CGRAPH_SRC_LIST})    # 编译libCGraph动态库
# add_library(CGraph STATIC ${CGRAPH_SRC_LIST})    # 编译libCGraph静态库
