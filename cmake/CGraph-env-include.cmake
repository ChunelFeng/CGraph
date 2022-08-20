
# 本cmake文件，供三方引入CGraph引用，用于屏蔽系统和C++版本的区别

IF(APPLE)
    # 非mac平台，暂时不支持自动生成session信息
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -finline-functions -Wno-deprecated-declarations -Wno-c++17-extensions")
    add_definitions(-D_GENERATE_SESSION_)
    add_definitions(-D_ENABLE_LIKELY_)
ELSEIF(UNIX)
    # linux平台，加入多线程内容
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread -Wno-format-overflow")
    add_definitions(-D_ENABLE_LIKELY_)
ELSEIF(WIN32)
    # windows平台，加入utf-8设置。否则无法通过编译
    add_definitions(/utf-8)

    # 禁止两处warning级别提示
    add_compile_options(/wd4996)
    add_compile_options(/wd4267)
ENDIF()
