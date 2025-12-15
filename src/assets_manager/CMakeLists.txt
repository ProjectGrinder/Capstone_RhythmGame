file(GLOB
    ASSETS_MANAGER_FILES
        "./*.c"
        "./*.h"
)

add_library(assets_manager
    STATIC
        ${ASSETS_MANAGER_FILES}
)

