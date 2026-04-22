enable_language(C)
file(GLOB
    AUDIO_FILES
        "./*.c"
        "./*.h"
)

add_library(os_audio
        STATIC
        ${AUDIO_FILES}
)
