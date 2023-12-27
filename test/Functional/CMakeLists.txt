
set(CGRAPH_FUNCTIONAL_LIST
        test-functional-01
        test-functional-02
        )

foreach(func ${CGRAPH_FUNCTIONAL_LIST})
    add_executable(${func}
            $<TARGET_OBJECTS:CGraph>
            ${func}.cpp
            )
endforeach()
