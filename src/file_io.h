#pragma once

/// @brief Read the file at the given file path into a buffer. The returned pointer must be freed by
/// the caller.
/// @param file_path
/// @param file_buffer_size_ptr
/// @return
unsigned char* file_to_buffer(const char* const file_path, long* const file_buffer_size_ptr);
