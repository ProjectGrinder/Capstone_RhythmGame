file(GLOB
    ECS_FILES
        "./*.cpp"
)

add_library(ecs
    STATIC
        ${ECS_FILES}
)