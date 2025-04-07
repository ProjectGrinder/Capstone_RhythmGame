file(GLOB
    ENGINE_FILES
        "./*.cpp"
)

add_library(engine
    STATIC
        ${ENGINE_FILES}
)

target_link_libraries(engine
    PRIVATE
        utils
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)