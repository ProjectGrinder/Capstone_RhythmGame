file(GLOB
    RENDERER_FILES
        "./*.cpp"
)

add_library(renderer
    STATIC
        ${RENDERER_FILES}
)
