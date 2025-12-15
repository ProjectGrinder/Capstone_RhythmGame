file(GLOB
    ENGINE_FILES
        "./*.cpp"
)

add_library(engine
    STATIC
        ${ENGINE_FILES}
)
