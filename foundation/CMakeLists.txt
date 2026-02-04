
add_library(foundation STATIC
        mem/alloc_operation.h
        mem/cell.h
        atom.h
        atom_factory.h
        atom_factory_impl.h
        defines.h
        mem/free_node.h
        mem/garbage_collector_visitor.h
        mem/garbage_collector_acceptor.h
        mem/allocator_from_arena.cpp
        mem/allocator_from_arena.h
        mem/allocator_from_garbage_collected.cpp
        mem/allocator_from_garbage_collected.h
        mem/simple_memory_management.h
        mem/simple_memory_management.cpp
        mem/memory_management.h
        foundation_impl.cpp
        foundation_impl.h
        foundation.h
        debug_output.h
)


target_include_directories(foundation
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${boost_di_SOURCE_DIR}/include/
)
