file(GLOB
        DIRECTX_FILES
        "./*.cpp"
        "./*.hpp"
        "./*.h"
)

add_library(directx_lib
        STATIC
        ${DIRECTX_FILES}
)

target_link_libraries(directx_lib
        PRIVATE
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)