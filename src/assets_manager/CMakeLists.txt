file(GLOB
    ASSETS_MANAGER_FILES
        "./*.c"
        "./*.h"
)

add_library(assets_manager
    STATIC
        ${ASSETS_MANAGER_FILES}
)

target_link_libraries(assets_manager
        PRIVATE
            directx
            user32
            gdi32
            kernel32
            dinput8
            winmm
)
