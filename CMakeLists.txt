cmake_minimum_required(VERSION 3.30)
project(Capstone_RhythmGame)

set(CMAKE_CXX_STANDARD 20)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(src/engine)

add_executable(${PROJECT_NAME}
    WIN32
        src/main.cpp
)

# Our Libs
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        engine
)

# Windows Libs
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)

option(DISABLE_TEST
    "Disable Unit Test"
        FALSE
)

if (NOT DISABLE_TEST)
    enable_testing()
    add_subdirectory(test)
endif()
