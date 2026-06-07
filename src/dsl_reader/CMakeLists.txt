file(GLOB
    DSL_READER_FILES
        "./*.cpp"
)

add_library(dsl_reader
    STATIC
        ${DSL_READER_FILES}
)
