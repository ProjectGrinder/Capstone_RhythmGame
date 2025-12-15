file(GLOB
        DIRECTX_FILES
        "./*.cpp"
        "./*.hpp"
        "./*.h"
)

add_library(os_scene
        OBJECT
        ${DIRECTX_FILES}
)

target_link_libraries(os_scene
        PRIVATE
        scene
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)
