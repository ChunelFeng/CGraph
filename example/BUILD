# NOET: test all the examples at a time
test_suite(
    name = "test_all_examples",
    tests = [":E01-AutoPilot",":E02-MockGUI",":E03-ThirdFlow",":E04-MapReduce"],
)

# test-1: E01-AutoPilot
cc_test (
    name = "E01-AutoPilot",
    srcs = ["E01-AutoPilot.cpp"],
    copts = ["-Isrc/"],
    deps = ["//src:CGraph",],
)

# test-2: E02-MockGUI
cc_test (
    name = "E02-MockGUI",
    srcs = ["E02-MockGUI.cpp"],
    copts = ["-Isrc/"],
    deps = ["//src:CGraph",],
)

# test-3: E03-ThirdFlow
cc_test (
    name = "E03-ThirdFlow",
    srcs = ["E03-ThirdFlow.cpp"],
    copts = ["-Isrc/"],
    deps = ["//src:CGraph",],
)

# test-4: E04-MapReduce
cc_test (
    name = "E04-MapReduce",
    srcs = ["E04-MapReduce.cpp"],
    copts = ["-Isrc/"],
    deps = ["//src:CGraph",],
)
