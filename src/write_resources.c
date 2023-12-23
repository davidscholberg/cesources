#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "write_resources.h"

// This is the size that a single byte takes up in the braced initializer list of an embedded
// resource file. An example of one of these byte elements is the following: "'\012',".
#define octal_escape_element_size 7
#define path_max_length 4096

/// @brief Format string for defining the unsigned char array and size variables for a given
/// resource file. A null byte is placed at the end of the array so that the array can be used as a
/// C string by just casting it to char*.
static const char* const resource_var_definition_fmt =
    "const unsigned char %s[] = {%s0};\n"
    "const int %s_size = %ld;\n\n";

bool write_resources(
    const char* input_dir,
    const char* output_dir,
    const char* resources_filename,
    const char** const resource_filenames,
    const int resource_file_count
) {
    // Set defaults
    if (!input_dir) {
        input_dir = "";
    }
    if (!output_dir) {
        output_dir = "";
    }
    if (!resources_filename) {
        resources_filename = "resources.c";
    }

    // Make sure input and output dirs have trailing slashes
    const int input_dir_length = strlen(input_dir);
    const int output_dir_length = strlen(output_dir);
    if (input_dir_length > 0 &&
        (input_dir[input_dir_length - 1] != '/' && input_dir[input_dir_length - 1] != '\\')) {
        fprintf(stderr, "error: input directory is missing trailing slash: \"%s\"\n", input_dir);
        return false;
    }
    if (output_dir_length > 0 &&
        (output_dir[output_dir_length - 1] != '/' && output_dir[output_dir_length - 1] != '\\')) {
        fprintf(stderr, "error: output directory is missing trailing slash: \"%s\"\n", output_dir);
        return false;
    }

    // Make sure max resource file path doesn't exceed max file path size
    int max_resource_filename_length = 0;
    for (int i = 0; i < resource_file_count; i++) {
        const int current_resource_filename_length = strlen(resource_filenames[i]);
        if (current_resource_filename_length > max_resource_filename_length) {
            max_resource_filename_length = current_resource_filename_length;
        }
    }
    if (input_dir_length + max_resource_filename_length >= path_max_length) {
        fprintf(
            stderr,
            "error: max resource path length exceeds max path length of %d:\n",
            path_max_length
        );
        return false;
    }

    // Create resources file path.
    if (strlen(output_dir) + strlen(resources_filename) >= path_max_length) {
        fprintf(
            stderr,
            "error: output path exceeds max path length of %d:\n"
            "    output directory:   \"%s\"\n"
            "    resources filename: \"%s\"\n",
            path_max_length,
            output_dir,
            resources_filename
        );
        return false;
    }
    char resources_file_path[path_max_length];
    strcpy(resources_file_path, output_dir);
    strcat(resources_file_path, resources_filename);

    // Open resources file and write the resource files to it.
    FILE* resources_file = fopen(resources_file_path, "w");
    if (!resources_file) {
        fprintf(stderr, "error: couldn't open output resources file \"%s\"\n", resources_file_path);
        return false;
    }
    char resource_file_path[path_max_length];
    strcpy(resource_file_path, input_dir);
    for (int i = 0; i < resource_file_count; i++) {
        // Build full resource file path
        resource_file_path[input_dir_length] = 0;
        strcat(resource_file_path, resource_filenames[i]);

        // Read resource file into buffer
        long file_buffer_size = 0;
        unsigned char* const file_buffer = file_to_buffer(resource_file_path, &file_buffer_size);
        if (!file_buffer) {
            fclose(resources_file);
            return false;
        }

        // Convert file buffer into initializer list of octal escape characters.
        const int file_octal_array_str_size = (file_buffer_size * octal_escape_element_size) + 1;
        char* file_octal_array_str = malloc(file_octal_array_str_size);
        if (!file_octal_array_str) {
            fprintf(
                stderr,
                "error: couldn't allocate space for octal string of file \"%s\"\n",
                resource_file_path
            );
            free(file_buffer);
            fclose(resources_file);
            return false;
        }
        char* file_octal_ptr = file_octal_array_str;
        for (int j = 0; j < file_buffer_size; j++) {
            const int bytes_written = snprintf(
                file_octal_ptr,
                octal_escape_element_size + 1,
                "'\\%.3o',",
                file_buffer[j]
            );
            if (bytes_written != octal_escape_element_size) {
                fprintf(
                    stderr,
                    "error: conversion to octal failed for file \"%s\"\n",
                    resource_file_path
                );
                free(file_octal_array_str);
                free(file_buffer);
                fclose(resources_file);
                return false;
            }

            file_octal_ptr += octal_escape_element_size;
        }
        free(file_buffer);

        // Determine resource variable name
        const int resource_file_name_size = strlen(resource_filenames[i]);
        char resource_var_name[resource_file_name_size + 1];
        strcpy(resource_var_name, resource_filenames[i]);
        char* period_ptr = strchr(resource_var_name, '.');
        if (period_ptr) {
            *period_ptr = '_';
        }

        // Write resource to resources file
        int status = fprintf(
            resources_file,
            resource_var_definition_fmt,
            resource_var_name,
            file_octal_array_str,
            resource_var_name,
            file_buffer_size
        );
        if (status <= 0) {
            fprintf(
                stderr,
                "error: failed to write resource file \"%s\" to resources file \"%s\"\n",
                resource_file_path,
                resources_file_path
            );
            free(file_octal_array_str);
            fclose(resources_file);
            return false;
        }

        // Clean up this iteration
        free(file_octal_array_str);
    }

    // Close resources file and return
    fclose(resources_file);
    return true;
}
