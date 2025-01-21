#ifndef FILE_IO_H
#define FILE_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/**
 * \brief Reads the entire contents of a file into memory.
 *        The caller is responsible for freeing the returned buffer.
 * \param filename Path to the file
 * \param out_size Pointer to a size_t variable to store the file length (optional)
 * \return A dynamically allocated buffer containing the file text, or NULL on failure
 */
char* read_file_to_string(const char* filename, size_t* out_size);

/**
 * \brief Writes a string to a file, overwriting it if it already exists.
 * \param filename Path to the output file
 * \param content Null-terminated string to write
 * \return 0 on success, nonzero on error
 */
int write_string_to_file(const char* filename, const char* content);

#ifdef __cplusplus
}
#endif

#endif /* FILE_IO_H */
