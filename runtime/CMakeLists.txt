
add_library(runtime STATIC
        lisp_runtime.h
        s_expr.h
        cell_factory.h
        simple_lisp_runtime.cpp
        simple_lisp_runtime.h
        simple_cell_factory.cpp
        simple_cell_factory.h
        cell_adaptor.h
        parser/parser.h
        parser/stream.h
        utilities/utilities.h
        utilities/utilities_string.cpp
)


target_include_directories(runtime
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

)


target_link_libraries(runtime
        PUBLIC
        foundation
)
