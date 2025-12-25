file(GLOB
        DIRECTX_FILES
        "./*.cpp"
        "./*.hpp"
        "./*.h"
)

add_library(directx
        STATIC
        ${DIRECTX_FILES}
)

target_link_libraries(directx
        PRIVATE
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)
