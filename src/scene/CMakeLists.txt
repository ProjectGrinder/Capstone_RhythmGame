file(GLOB
    SCENE_FILES
        "./*.cpp"
)

add_library(scene
    STATIC
        ${SCENE_FILES}
)

target_link_libraries(scene
    PRIVATE
        engine
        os_utils
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)
