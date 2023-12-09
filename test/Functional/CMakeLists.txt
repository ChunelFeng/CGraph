
set(CGRAPH_FUNCTIONAL_LIST
        )

foreach(func ${CGRAPH_FUNCTIONAL_LIST})
    add_executable(${func}
            $<TARGET_OBJECTS:CGraph>
            ${func}.cpp
            )
endforeach()
