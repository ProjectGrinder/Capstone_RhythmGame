find_program(FXC fxc DOC "fx shader compiler")

if (NOT FXC)
    message(FATAL_ERROR "FX Shader compiler not found")
endif()

message(STATUS "Found FXC: ${FXC}")

set(FXC_FLAGS "/nologo" "/Vi")

if (("${CMAKE_BUILD_TYPE}" STREQUAL "Debug") AND ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC"))
    list(APPEND FXC_FLAGS /Zi /Od)
endif()

file(GLOB_RECURSE VS_SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/vs/*.hlsl")
file(GLOB_RECURSE PS_SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/ps/*.hlsl")
file(GLOB_RECURSE CS_SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/cs/*.hlsl")

function(add_shaders SHADERS SHADER_TARGET)
    foreach(SHADER_FILE IN LISTS ${SHADERS})
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
        set(OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_TARGET}/${SHADER_NAME}.cso")

        add_custom_command(
                OUTPUT ${OUTPUT_FILE}
                COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_TARGET}"
                COMMAND ${FXC} ${FXC_FLAGS} /T "${SHADER_TARGET}_5_0" /Fo "${OUTPUT_FILE}" "${SHADER_FILE}"
                DEPENDS "${SHADER_FILE}"
                COMMENT "Compiling ${SHADER_FILE} -> ${OUTPUT_FILE} (${SHADER_TARGET})"
                VERBATIM
        )

        list(APPEND COMPILED_SHADERS ${OUTPUT_FILE})
    endforeach()
    set(COMPILED_SHADERS ${COMPILED_SHADERS} PARENT_SCOPE)
endfunction()

set(COMPILED_SHADERS)

add_shaders(VS_SHADERS vs)
add_shaders(PS_SHADERS ps)
add_shaders(CS_SHADERS cs)

add_custom_target(compile_shaders ALL DEPENDS ${COMPILED_SHADERS})
