
add_library(foundation STATIC
        memory_management.h
        memory_management.cpp
        mem/alloc_operation.h
        mem/cell.h
        atom.h
        atom_factory.h
        atom_factory_impl.h
        defines.h
)


target_include_directories(foundation
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
