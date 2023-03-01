# tutorial中的内容

set(CGRAPH_TUTORIAL_LIST
        T00-HelloCGraph
        T01-Simple
        T02-Cluster
        T03-Region
        T04-Complex
        T05-Param
        T06-Condition
        T07-MultiPipeline
        T08-Template
        T09-Aspect
        T10-AspectParam
        T11-Singleton
        T12-Function
        T13-Daemon
        T14-Hold
        T15-ElementParam
        T16-MessageSendRecv
        T17-MessagePubSub
        T18-Event

        # 以下为工具类tutorial
        TU01-ThreadPool
        TU02-Lru
        TU03-Trie
        TU04-Timer
        TU05-Distance
        )

foreach(tut ${CGRAPH_TUTORIAL_LIST})
    add_executable(${tut} ${tut}.cpp $<TARGET_OBJECTS:CGraph>)
endforeach()
