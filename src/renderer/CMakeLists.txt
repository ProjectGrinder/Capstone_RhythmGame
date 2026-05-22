file(GLOB
    RENDERER_FILES
        "./*.cpp"
        "./*.c"
)

add_library(renderer
    STATIC
        ${RENDERER_FILES}
)

