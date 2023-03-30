#ifndef _PERFECTION_LEXER_H
#define _PERFECTION_LEXER_H

// Represents a lexer.
typedef struct _perf_lexer_t
{
    const char* src;            // Source code
    const char* token_start;    // Start of the current token
    const char* current_ch;     // Current character

    uint32_t line_number;       // Current line number
    uint32_t column_number;     // Current column number
} perf_lexer_t;

/**
 * @brief Initializes a lexer.
 *
 * @param lexer The lexer to initialize.
 * @return PERF_RES_OK if the lexer was initialized successfully.
 */
perf_result_t perf_lexer_init(perf_lexer_t *lexer);

/**
 * @brief Parses the given code and converts it into a token stream.
 * 
 * @param lexer The lexer to use.
 * @param src The source code to parse.

 * @return PERF_RES_OK if the source code was parsed successfully.
 */
perf_result_t perf_lexer_digest(perf_lexer_t *lexer, const char* src, perf_token_t **tokens, int32_t *token_count, const char** error);

/**
 * @brief Prints out the error message.
 * 
 * @param lexer The lexer to use.
 * @param error The error message to print.
 * 
 * @return PERF_RES_OK if the error message was printed successfully.
 */
perf_result_t perf_lexer_error_print(perf_lexer_t *lexer, const char *error);

#endif // _PERFECTION_LEXER_H