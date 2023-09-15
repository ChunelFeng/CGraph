# 对应 example 中的内容
set(CGRAPH_EXAMPLE_LIST
        E01-AutoPilot
        )

foreach(example ${CGRAPH_EXAMPLE_LIST})
    add_executable(${example}
            $<TARGET_OBJECTS:CGraph>
            ${example}.cpp
            )
endforeach()