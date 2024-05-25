CGRAPH_FUNCTIONAL_LIST = [
        "test-functional-01",
        "test-functional-02",
        "test-functional-03",
        "test-functional-04",
        "test-functional-05",
        ]

[
    cc_test(
        name = "%s" % fun_name,
        srcs = ["%s.cpp" % fun_name],

        deps = ["//src:CGraph",
                "//test/_Materials:test_materials",],
    )
    for fun_name in CGRAPH_FUNCTIONAL_LIST
]