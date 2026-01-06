

add_library(app STATIC
            app.cpp
            app.h
)

target_include_directories(app
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(app
        PUBLIC
        runtime
)