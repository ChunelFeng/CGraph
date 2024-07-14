load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "CGraph",
    srcs = glob(["**/*.cpp"]),
    hdrs = glob(["**/*.h", "**/*.inl"]),
    copts = select({
        "@platforms//os:linux": ["-D_ENABLE_LIKELY_"],
        "@platforms//os:macos": ["-D_ENABLE_LIKELY_"],
        "//conditions:default": [],
    }),
    visibility = ["//visibility:public"],
)
