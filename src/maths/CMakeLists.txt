file(GLOB
    MATHS_FILES
        "./*.cpp"
)

add_library(maths
    SHARED
        ${MATHS_FILES}
)