file(GLOB
    MATHS_FILES
        "./*.cpp"
)

add_library(maths
    STATIC
        ${MATHS_FILES}
)