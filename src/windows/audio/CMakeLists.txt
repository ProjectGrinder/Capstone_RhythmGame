enable_language(C)
file(GLOB
    AUDIO_FILES
        "./*.c"
        "./*.h"
)

add_library(audio
        STATIC
        ${AUDIO_FILES}
)

target_link_libraries(audio
        PRIVATE
        os_utils
        user32
        gdi32
        kernel32
        d3d11
        dxgi
        dinput8
        winmm
)
