load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "CGraph",
    srcs = glob(["**/*.cpp"]),
    hdrs = glob(["**/*.h", "**/*.inl"]),
    copts = [
             "-D_CGRAPH_SILENCE",
             "-D_CGRAPH_SHOW_THREAD_METRICS_",
             "-D_ENABLE_LIKELY_",
            ],
    linkstatic = 0,
    visibility = ["//visibility:public"],
)
