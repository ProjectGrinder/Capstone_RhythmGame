add_subdirectory(asm)
enable_language(C)

file(GLOB
        WINDOWS_UTILS_FILES
        "./*.c"
        "./*.h"
)

add_library(os_utils
        OBJECT
        ${WINDOWS_UTILS_FILES}
)
