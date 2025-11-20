add_subdirectory(utils)
add_subdirectory(directx)

# OS Level Code will be written in C
file(GLOB
    WINDOWS_FILES
        "./*.c"
        "./*.h"
)

add_executable(windows_main
        WIN32
            ${WINDOWS_FILES}
)

target_link_libraries(windows_main
        PRIVATE
            ${LIBS}
            directx_lib
            user32
            gdi32
            kernel32
            d3d11
            dxgi
            dinput8
            winmm
)

add_dependencies(windows_main compile_shaders)