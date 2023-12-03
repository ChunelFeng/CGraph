
set(CGRAPH_FUNCTIONAL_LIST
        test-functional-01
        )


foreach(func ${CGRAPH_FUNCTIONAL_LIST})
    add_executable(${func}
            # 在自己的工程中引入CGraph功能，仅需引入 CGraph-env-include.cmake 后，加入这一句话即可
            $<TARGET_OBJECTS:CGraph>
            ${func}.cpp
            )
endforeach()
