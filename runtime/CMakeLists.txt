
add_library(runtime STATIC
        lisp_runtime.h
        s_expr.h
        cell_factory.h
        simple_lisp_runtime.cpp
        simple_lisp_runtime.h
        simple_cell_factory.cpp
        simple_cell_factory.h
        mem/alloc_operation.h
        runtime_defines.h
)


target_include_directories(runtime
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)