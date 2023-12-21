#include <stdio.h>
#include <stdlib.h>

#include "file_io.h"

/// @brief Get the file size of the given file pointer. Note that the file pointer will be reset
/// back to the beginning if the function is successful.
/// @param file
/// @return
long file_size(FILE* file) {
    int ret = fseek(file, 0, SEEK_END);
    if (ret) {
        return ret;
    }

    long size = ftell(file);
    if (size == -1) {
        return size;
    }

    ret = fseek(file, 0, SEEK_SET);
    if (ret) {
        return ret;
    }

    return size;
}

unsigned char* file_to_buffer(const char* const file_path, long* const file_buffer_size_ptr) {
    *file_buffer_size_ptr = 0;

    // Get file size.
    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "error: couldn't open file path \"%s\"\n", file_path);
        return NULL;
    }
    long file_buffer_size = file_size(file);
    if (file_buffer_size <= 0) {
        fprintf(stderr, "error: couldn't determine file size of file \"%s\"\n", file_path);
        fclose(file);
        return NULL;
    }

    // Read file into buffer.
    unsigned char* file_buffer = malloc(file_buffer_size);
    if (!file_buffer) {
        fprintf(stderr, "error: couldn't allocate memory for file \"%s\"\n", file_path);
        fclose(file);
        return NULL;
    }
    int bytes_read = fread(file_buffer, 1, file_buffer_size, file);
    if (ferror(file) || bytes_read != file_buffer_size) {
        fprintf(stderr, "error: couldn't read file into memory: \"%s\"\n", file_path);
        free(file_buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    *file_buffer_size_ptr = file_buffer_size;
    return file_buffer;
}
