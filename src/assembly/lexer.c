c

#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 16

static char* safe_strdup(const char* src) {
    if (!src) return NULL;
    size_t len = strlen(src);
    char* dst = (char*)malloc(len + 1);
    if (dst) {
        memcpy(dst, src, len + 1);
    }
    return dst;
}

int init_token_list(TokenList* list) {
    if (!list) return -1;
    list->data = (Token*)malloc(INITIAL_CAPACITY * sizeof(Token));
    if (!list->data) return -2;
    list->size = 0;
    list->capacity = INITIAL_CAPACITY;
    return 0;
}

int append_token(TokenList* list, TokenType type, const char* text) {
    if (!list || !text) return -1;

    // Resize if needed
    if (list->size >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        Token* new_data = (Token*)realloc(list->data, new_capacity * sizeof(Token));
        if (!new_data) return -2;
        list->data = new_data;
        list->capacity = new_capacity;
    }

    // Copy text
    char* token_text = safe_strdup(text);
    if (!token_text) return -3;

    // Assign
    list->data[list->size].type = type;
    list->data[list->size].text = token_text;
    list->size++;

    return 0;
}

void free_token_list(TokenList* list) {
    if (!list) return;
    for (size_t i = 0; i < list->size; i++) {
        free(list->data[i].text);
    }
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

static int is_all_digits(const char* str) {
    // Return 1 if all characters in str are digits, else 0
    for (size_t i = 0; i < strlen(str); i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

void lex_line(const char* line, TokenList* list) {
    if (!line || !list) return;

    // Check for comment start first
    // We'll handle `#` or `//` as immediate comment tokens
    // If found, the rest of the line is a comment
    // e.g. # This is a comment
    // or   // Another comment
    const char* comment_ptr_hash = strchr(line, '#');
    const char* comment_ptr_slash = strstr(line, "//");

    int comment_start = -1;
    if (comment_ptr_hash) {
        comment_start = (int)(comment_ptr_hash - line);
    }
    if (comment_ptr_slash) {
        int slash_pos = (int)(comment_ptr_slash - line);
        if (comment_start < 0 || slash_pos < comment_start) {
            comment_start = slash_pos;
        }
    }

    char buffer[256];
    int buffer_index = 0;
    int i = 0;
    int len = (int)strlen(line);

    while (i < len) {
        // If we've reached the start of a comment, everything else is a comment
        if (comment_start >= 0 && i == comment_start) {
            // Append a single comment token
            append_token(list, TOKEN_COMMENT, &line[comment_start]);
            return; // Done processing this line
        }

        // Skip whitespace
        if (isspace((unsigned char)line[i])) {
            i++;
            continue;
        }

        // Collect a chunk of non-whitespace as a token
        buffer_index = 0;
        while (i < len && !isspace((unsigned char)line[i]) && i != comment_start && buffer_index < 255) {
            buffer[buffer_index++] = line[i++];
        }
        buffer[buffer_index] = '\0';

        // Determine token type
        if (is_all_digits(buffer)) {
            // It's an integer
            append_token(list, TOKEN_INTEGER, buffer);
        } else {
            // We'll consider it a GATE token, though it might be unknown or a reserved word
            // Optionally, you could refine the logic to check for valid gate keywords
            append_token(list, TOKEN_GATE, buffer);
        }
    }
}
