find_package(GTest REQUIRED)
include(GoogleTest)

file(GLOB
    TEST_FILES
        ./*.cpp
)

add_executable(unit_test
    ${TEST_FILES}
 "input_handler_test.cpp")

target_link_libraries(unit_test
    PRIVATE
        engine
        GTest::GTest GTest::Main
)

gtest_discover_tests(unit_test)