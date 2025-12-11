add_subdirectory(asm)

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
            #os_asm_utils
            user32
            gdi32
            kernel32
            winmm
)