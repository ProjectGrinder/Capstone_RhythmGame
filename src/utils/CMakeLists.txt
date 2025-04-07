file(GLOB
    UTILS_FILES
        "./*.cpp"
)

add_library(utils
    STATIC
        ${UTILS_FILES}
)