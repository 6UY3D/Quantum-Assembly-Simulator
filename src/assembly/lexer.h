c

#ifndef LEXER_H
#define LEXER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * \brief Token types for our quantum assembly language.
 */
typedef enum {
    TOKEN_GATE,      /**< e.g. "H", "X", "Y", "Z", "CNOT", "MEASURE", etc. */
    TOKEN_INTEGER,   /**< e.g. "0", "1", "2" for qubit indices */
    TOKEN_COMMENT,   /**< e.g. "// This is a comment" or "# Another comment" */
    TOKEN_UNKNOWN,   /**< Unrecognized token */
    TOKEN_EOF        /**< End of file/input */
} TokenType;

/**
 * \brief Represents a single token with type and textual content.
 */
typedef struct {
    TokenType type;
    char*     text;      /**< The original substring representing this token */
} Token;

/**
 * \brief Represents a dynamic array of tokens, for convenience.
 */
typedef struct {
    Token*  data;
    size_t  size;
    size_t  capacity;
} TokenList;

/**
 * \brief Initializes a TokenList with default capacity.
 * \param list Pointer to an uninitialized TokenList
 * \return 0 on success, nonzero on failure
 */
int init_token_list(TokenList* list);

/**
 * \brief Appends a new token to the TokenList.
 * \param list Pointer to a TokenList
 * \param type TokenType
 * \param text Text to store (the function copies the text)
 * \return 0 on success, nonzero on failure
 */
int append_token(TokenList* list, TokenType type, const char* text);

/**
 * \brief Frees all resources in a TokenList.
 * \param list Pointer to a TokenList
 */
void free_token_list(TokenList* list);

/**
 * \brief Performs lexical analysis on a single line of quantum assembly code.
 * \param line C-string representing one line from a `.qasm` file
 * \param list Pointer to a TokenList to which tokens will be appended
 */
void lex_line(const char* line, TokenList* list);

#ifdef __cplusplus
}
#endif

#endif /* LEXER_H */
