c

#include "file_io.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char* read_file_to_string(const char* filename, size_t* out_size) {
    if (!filename) return NULL;

    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        return NULL;
    }

    // Seek to end to get file size
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    if (fsize < 0) {
        fclose(fp);
        return NULL;
    }
    rewind(fp);

    char* buffer = (char*)malloc(fsize + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    size_t read_bytes = fread(buffer, 1, fsize, fp);
    fclose(fp);

    // Null-terminate
    buffer[read_bytes] = '\0';
    if (out_size) {
        *out_size = read_bytes;
    }
    return buffer;
}

int write_string_to_file(const char* filename, const char* content) {
    if (!filename || !content) return -1;

    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return -2;
    }
    size_t len = strlen(content);
    size_t written = fwrite(content, 1, len, fp);
    fclose(fp);

    return (written == len) ? 0 : -3;
}
