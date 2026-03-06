file(GLOB
    RENDERER_FILES
        "./*.cpp"
)

add_library(renderer
    STATIC
        ${RENDERER_FILES}
)

target_link_libraries(
        assets_manager
)