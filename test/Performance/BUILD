CGRAPH_PERFORMANCE_LIST = [
        "test-performance-01",
        "test-performance-02",
        "test-performance-03",
        "test-performance-04",
        ]

[
    cc_binary(
        name = "%s" % perf_name,
        srcs = ["%s.cpp" % perf_name],
        copts = ["-Isrc/"],
        deps = ["//src:CGraph",
                "//test/_Materials:test_materials",],
    )
    for perf_name in CGRAPH_PERFORMANCE_LIST
]