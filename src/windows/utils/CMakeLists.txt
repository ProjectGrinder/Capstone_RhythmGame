file(GLOB
        WINDOWS_UTILS_FILES
        "./*.c"
        "./*.h"
)

add_library(os_utils
        STATIC
        ${WINDOWS_UTILS_FILES}
)

target_link_libraries(os_utils
        PRIVATE
            user32
            gdi32
            kernel32
            d3d11
            dxgi
            dinput8
            winmm
)