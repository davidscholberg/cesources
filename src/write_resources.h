#pragma once

#include <stdbool.h>

/// @brief Take the input resource files, generate a C source file with those resource files
/// embedded as unsigned char buffers, and write this file to the output directory with the
/// specified name.
/// @param input_dir
/// @param output_dir
/// @param resources_filename
/// @param resource_filenames
/// @param resource_file_count
/// @return
bool write_resources(
    const char* input_dir,
    const char* output_dir,
    const char* resources_filename,
    const char** const resource_filenames,
    const int resource_file_count
);
