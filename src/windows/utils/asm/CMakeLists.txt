enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_OBJECT_FORMAT win64)
file(GLOB
        ASM_UTILS_FILES
        "./*.asm"
)

add_library(os_asm_utils
        OBJECT
        ${ASM_UTILS_FILES}
)
