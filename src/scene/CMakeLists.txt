file(GLOB
    SCENE_FILES
        "./*.cpp"
)

add_library(scene
    STATIC
        ${SCENE_FILES}
)
