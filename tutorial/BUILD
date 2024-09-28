cc_library(
    name = "tutorial_header",
    srcs = glob(["MyGAspect/**/*.h",
                "MyGCondition/**/*.h",
                "MyGDaemon/**/*.h",
                "MyGEvent/**/*.h",
                "MyGMutable/**/*.h",
                "MyGNode/**/*.h",
                "MyParams/**/*.h",
                "MyUtils/**/*.h",]),
    visibility = ["//visibility:public"],
)

CGRAPH_TUTORIAL_LIST = [
         "T00-HelloCGraph",
         "T01-Simple",
         "T02-Cluster",
         "T03-Region",
         "T04-Complex",
         "T05-Param",
         "T06-Condition",
         "T07-MultiPipeline",
         "T08-Template",
         "T09-Aspect",
         "T10-AspectParam",
         "T11-Singleton",
         "T12-Function",
         "T13-Daemon",
         "T14-Hold",
         "T15-ElementParam",
         "T16-MessageSendRecv",
         "T17-MessagePubSub",
         "T18-Event",
         "T19-Cancel",
         "T20-YieldResume",
         "T21-MultiCondition",
         "T22-Timeout",
         "T23-Some",
         "T24-Fence",
         "T25-Coordinator",
         "T26-Mutable",
         "T27-Trim",
        ]

[
    cc_binary(
        name = "%s" % tutorial_name,
        srcs = ["%s.cpp" % tutorial_name],
        copts = ["-Isrc/"],
        deps = [":tutorial_header","//src:CGraph"],
    )
    for tutorial_name in CGRAPH_TUTORIAL_LIST
]