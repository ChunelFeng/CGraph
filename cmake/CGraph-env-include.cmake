
# 本cmake文件，供三方引入CGraph引用，用于屏蔽系统和C++版本的区别

# 根据当前 CGraph-env-include.cmake 文件的位置，定位CGraph整体工程的路径
# 从而解决并兼容了直接引入和三方库引入的路径不匹配问题
get_filename_component(CGRAPH_PROJECT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

set(INSTALL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin" CACHE PATH "Installation directory for executables")
set(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")
set(INSTALL_INC_DIR "${CMAKE_INSTALL_PREFIX}/include" CACHE PATH "Installation directory for headers")
set(INSTALL_MAN_DIR "${CMAKE_INSTALL_PREFIX}/share/man" CACHE PATH "Installation directory for manual pages")
set(INSTALL_PKGCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/share/pkgconfig" CACHE PATH "Installation directory for pkgconfig (.pc) files")

set(CGRAPH_PROJECT_ROOT_DIR "${CGRAPH_PROJECT_CMAKE_DIR}/..")
file(GLOB_RECURSE CGRAPH_PROJECT_SRC_LIST "${CGRAPH_PROJECT_ROOT_DIR}/src/*.cpp")
file(GLOB_RECURSE CGRAPH_PROJECT_HDR_LIST "${CGRAPH_PROJECT_ROOT_DIR}/src/*.h" "${CGRAPH_PROJECT_ROOT_DIR}/src/*.inl")

IF(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -O2 \
        -finline-functions -Wno-deprecated-declarations -Wno-c++17-extensions")
    add_definitions(-D_ENABLE_LIKELY_)
ELSEIF(UNIX)
    # linux平台，加入多线程内容
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread -Wno-format-overflow")
    add_definitions(-D_ENABLE_LIKELY_)
ELSEIF(WIN32)
    IF(MSVC)
		set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
        # windows平台，加入utf-8设置。否则无法通过编译
        add_definitions(/utf-8)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

        # 禁止几处warning级别提示
        add_compile_options(/wd4996)
        add_compile_options(/wd4267)
        add_compile_options(/wd4018)
    ENDIF()
    # 本工程也支持在windows平台上的mingw环境使用
ENDIF()

include_directories(${CGRAPH_PROJECT_ROOT_DIR}/src/)    # 直接加入"CGraph.h"文件对应的位置

# 以下三选一，本地编译执行，推荐OBJECT方式
# add_library(CGraph OBJECT ${CGRAPH_PROJECT_SRC_LIST})      # 通过代码编译
# add_library(CGraph SHARED ${CGRAPH_PROJECT_SRC_LIST})    # 编译libCGraph动态库
# add_library(CGraph STATIC ${CGRAPH_PROJECT_SRC_LIST})    # 编译libCGraph静态库
if(BUILD_SHARED_LIBS)
  add_library(CGraph SHARED ${CGRAPH_PROJECT_SRC_LIST}) 
else()
  add_library(CGraph STATIC ${CGRAPH_PROJECT_SRC_LIST})
endif()

if(NOT SKIP_INSTALL_LIBRARIES AND NOT SKIP_INSTALL_ALL )
    install(TARGETS CGraph
        RUNTIME DESTINATION "${INSTALL_BIN_DIR}"
        ARCHIVE DESTINATION "${INSTALL_LIB_DIR}"
        LIBRARY DESTINATION "${INSTALL_LIB_DIR}" )
endif()
if(NOT SKIP_INSTALL_HEADERS AND NOT SKIP_INSTALL_ALL )
foreach ( file ${CGRAPH_PROJECT_HDR_LIST} )
    get_filename_component( dir ${file} DIRECTORY )
	string(REGEX REPLACE "^${CGRAPH_PROJECT_ROOT_DIR}/src[/]?" "" output_path "${dir}")
    install(FILES ${file} DESTINATION "${INSTALL_INC_DIR}/${output_path}" )
endforeach()
endif()
