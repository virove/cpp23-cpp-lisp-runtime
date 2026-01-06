
add_library(runtime STATIC
        lisp_runtime.h
        s_expr.h
        s_expr.cpp
)


target_include_directories(runtime
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)