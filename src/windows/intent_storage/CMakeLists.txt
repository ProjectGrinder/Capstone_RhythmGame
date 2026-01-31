file(GLOB
        INTENT_FILES
        "./*.cpp"
        "./*.hpp"
        "./*.h"
)

add_library(os_intent
        STATIC
        ${INTENT_FILES}
)

target_link_libraries(os_intent
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
