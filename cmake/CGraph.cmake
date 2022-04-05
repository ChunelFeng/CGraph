
include(FetchContent)

FetchContent_Declare(
        CGraph                                                     # 库名称
        GIT_REPOSITORY https://github.com/ChunelFeng/CGraph.git    # git仓库地址信息
        GIT_TAG v1.8.6                                             # git版本信息
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src/             # 指定库下载地址
)

FetchContent_MakeAvailable(CGraph)
