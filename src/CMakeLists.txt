target_include_directories(
    cesources
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(
    cesources
    PRIVATE
    file_io.c
    file_io.h
    main.c
    write_resources.c
    write_resources.h
)
