find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB
    TEST_FILES
        ./*.cpp
)

add_executable(unit_test
    ${TEST_FILES}
)

list(APPEND
        LIBS
        os_utils
)

target_link_libraries(unit_test
    PRIVATE
        ${LIBS}
        GTest::GTest GTest::Main
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)

gtest_discover_tests(unit_test)