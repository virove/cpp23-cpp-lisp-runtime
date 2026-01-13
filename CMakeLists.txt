cmake_minimum_required(VERSION 3.31)
project(cpp23-cpp-lisp-runtime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_Declare(
        boost_di
        GIT_REPOSITORY https://github.com/boost-ext/di.git
        GIT_TAG        v1.3.2
)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(boost_di)

add_subdirectory(runtime)
add_subdirectory(app)
add_subdirectory(foundation)
add_subdirectory(tests)

add_executable( cpp23-cpp-lisp-runtime
        main.cpp
)

target_include_directories(cpp23-cpp-lisp-runtime PRIVATE
        ${boost_di_SOURCE_DIR}/include/
)

target_link_libraries(cpp23-cpp-lisp-runtime
        PRIVATE runtime app
)





