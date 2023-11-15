
# 对应tutorial中的内容
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
        T19-Cancel
        T20-YieldResume
        T21-MultiCondition
        T22-Timeout
        T23-Some
        T24-Fence
        T25-Coordinator
        T26-Mutable
        )


foreach(tut ${CGRAPH_TUTORIAL_LIST})
    add_executable(${tut}
            # 在自己的工程中引入CGraph功能，仅需引入 CGraph-env-include.cmake 后，加入这一句话即可
            $<TARGET_OBJECTS:CGraph>
            ${tut}.cpp
            )
endforeach()
