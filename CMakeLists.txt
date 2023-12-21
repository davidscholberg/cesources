cmake_minimum_required(VERSION 3.13)

project(
    cesources
    VERSION 0.1.0
    LANGUAGES C
)

add_executable(cesources)

# NOTE: for some reason, using target_compile_features to set the C standard to c99 had no effect, but the below works fine.
set_target_properties(
    cesources
    PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_compile_options(
    cesources
    PRIVATE
    -Wall
    -Wextra
    -Werror
    -Wpedantic
)

add_subdirectory(src)
