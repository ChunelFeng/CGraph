
set(CGRAPH_PERFORMANCE_LIST
        test-performance-01
        test-performance-02
        test-performance-03
        test-performance-04
        )

foreach(perf ${CGRAPH_PERFORMANCE_LIST})
    add_executable(${perf}
            # 在自己的工程中引入CGraph功能，仅需引入 CGraph-env-include.cmake 后，加入这一句话即可
            $<TARGET_OBJECTS:CGraph>
            ${perf}.cpp
            )
endforeach()
