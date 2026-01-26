project(UnitTests)

add_executable(UnitTests
        app_test.cpp
        simple_cell_factory_test.cpp
        atom_factory_test.cpp
        memory_management_test.cpp
        mocks/alloc_operation_mock.h
        mocks/memory_management_mock.h
        mocks/garbage_collector_acceptor_mock.h
        foundation_test.cpp
        mocks/atom_factory_mock.h
        mocks/foundation_mock.h
)

target_link_libraries(UnitTests
        PRIVATE
        runtime
        app
        foundation
        PRIVATE ws2_32
        GTest::gmock
        GTest::gtest_main
#        di
)

target_include_directories(UnitTests PRIVATE
        ${boost_di_SOURCE_DIR}/include/
)

include(GoogleTest)
gtest_discover_tests(UnitTests)