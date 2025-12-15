if(WIN32)
    include(FetchContent)

    FetchContent_Declare(
        nasm_bin
        URL https://www.nasm.us/pub/nasm/releasebuilds/3.01rc9/win64/nasm-3.01rc9-win64.zip
    )

    FetchContent_MakeAvailable(nasm_bin)

    set(NASM_EXE "${nasm_bin_SOURCE_DIR}/nasm.exe")
    set(CMAKE_ASM_NASM_COMPILER "${NASM_EXE}")
endif()