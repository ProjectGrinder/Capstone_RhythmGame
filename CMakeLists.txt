cmake_minimum_required(VERSION 3.30)
project(Capstone_RhythmGame)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 23)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(/W4 /WX)
#add_compile_options(/FAcs /O2 /Ob2 /Oi /Ot /GS- /Gy /DNDEBUG /Zi -flto)
add_compile_definitions(PROJECT_NAME=${PROJECT_NAME})


list(APPEND
    LIBS
        scene
        engine
)

include_directories("include")

option(DISABLE_TEST
    "Disable Unit Test"
        FALSE
)

file(GLOB
    SRC_FOLDERS
    RELATIVE
        "${PROJECT_SOURCE_DIR}/src/"
        "${PROJECT_SOURCE_DIR}/src/*"
)

foreach(SRC_FOLDER ${SRC_FOLDERS})
    if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/src/${SRC_FOLDER}")
        add_subdirectory("${PROJECT_SOURCE_DIR}/src/${SRC_FOLDER}")
    endif()
endforeach()

add_subdirectory(shaders)

enable_testing()
add_subdirectory("test")

add_custom_target(unit_testing
    COMMAND ${CMAKE_CTEST_COMMAND}
    COMMENT "[${PROJECT_NAME}] - Unit Testing"
    DEPENDS unit_test
)

if (NOT DISABLE_TEST)
    add_custom_target(building_project
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
        COMMENT "[${PROJECT_NAME}] - Building"
        DEPENDS unit_testing
    )
else()
    add_custom_target(building_project
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
        COMMENT "[${PROJECT_NAME}] - Building"
    )
endif()

#add_executable(${PROJECT_NAME}
#    WIN32
#        src/main.cpp
#)
#
#target_link_libraries(${PROJECT_NAME}
#    PRIVATE
#        ${LIBS}
#        user32
#        gdi32
#        kernel32
#        d3d11
#        dxgi
#        dinput8
#        winmm
#)
#
#add_dependencies(${PROJECT_NAME} compile_shaders)
