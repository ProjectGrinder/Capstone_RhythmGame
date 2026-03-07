file(
    GLOB
    VERTEX_FILES
    "./*.c"
    "./*.cpp"
    "./*.h"
    "./*.hpp"
)

add_library(
    vertex_generator
    STATIC
    ${VERTEX_FILES}
)
