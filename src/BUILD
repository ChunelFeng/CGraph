load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "CGraph",
    srcs = glob(["**/*.cpp"]),
    hdrs = glob(["**/*.h", "**/*.inl"]),
    copts = [
             "-D_CGRAPH_SILENCE",
             "-D_ENABLE_LIKELY_",
            ],
    visibility = ["//visibility:public"],
)
