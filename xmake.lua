-- project
set_project("CGraph")

-- set project version
set_version("2.1.1")

-- set language: c++17
set_languages("c++17")

-- set features on different platform
if is_plat("macosx") then
    add_defines("_GENERATE_SESSION_")
    add_defines("_ENABLE_LIKELY_")
elseif is_plat("linux") then
    add_defines("_ENABLE_LIKELY_")
    add_syslinks("pthread")
end

tutorial_list = {
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
        "TU01-ThreadPool",
        "TU02-Lru",
        "TU03-Trie",
        "TU04-Timer"
        "TU05-Distance"
}

-- add tutorial target one by one
for _, v in pairs(tutorial_list) do
    target(v)
        set_kind("binary")
        add_files("src/**.cpp", string.format("tutorial/%s.cpp", v))
end