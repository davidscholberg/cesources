cmake_minimum_required(VERSION 3.24)

project(
    cesources
    VERSION 0.1.0
    LANGUAGES C
)

add_executable(cesources)

target_compile_features(
    cesources
    PRIVATE
    c_std_17
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
