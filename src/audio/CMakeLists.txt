file(GLOB
    AUDIO_FILES 
        "./*.c"
)

add_library(audio
    STATIC
        ${AUDIO_FILES}
)
