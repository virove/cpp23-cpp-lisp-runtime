
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
        functions/builtin_function.h
        runtime.h
        functions/builtin_function.cpp
        runtime_impl.cpp
        runtime_impl.h
        forms/forms.h
        forms/form.h
        forms/forms_impl.cpp
        forms/forms_impl.h
        forms/quote_form.cpp
        forms/quote_form.h
        functions/functions.h
        functions/functions_impl.cpp
        functions/functions_impl.h
)


target_include_directories(runtime
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}

)


target_link_libraries(runtime
        PUBLIC
        foundation
)
