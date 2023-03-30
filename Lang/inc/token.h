#ifndef _PERFECTION_TOKEN_H
#define _PERFECTION_TOKEN_H

/**
 * Represents the type of the token.
 */
typedef enum _perf_e_token_type_t
{
    TOKEN_LEFT_PARENTHESES,     // (
    TOKEN_RIGHT_PARENTHESES,    // )
    TOKEN_LEFT_BRACE,           // {
    TOKEN_RIGHT_BRACE,          // }
    TOKEN_COMMA,                // ,
    TOKEN_PERIOD,               // .
    TOKEN_SEMICOLON,            // ;
    TOKEN_COLON,                // :
    TOKEN_MINUS,                // -
    TOKEN_PLUS,                 // +
    TOKEN_SLASH,                // /
    TOKEN_ASTERISK,             // *
    TOKEN_PERCENT,              // %
    TOKEN_AMPERSAND,            // &

    TOKEN_EXCLAIM,              // !
    TOKEN_EXCLAIM_EQUAL,        // !=
    TOKEN_EQUAL,                // =
    TOKEN_EQUAL_EQUAL,          // ==
    TOKEN_GREATER,              // >
    TOKEN_GREATER_EQUAL,        // >=
    TOKEN_LESS,                 // <
    TOKEN_LESS_EQUAL,           // <=

    TOKEN_IDENTIFIER,           // Identifier
    TOKEN_STRING,               // String
    TOKEN_NUMBER,               // Number
    TOKEN_INTEGER,              // Integer

    TOKEN_KEYWORD_FUNC,         // func 
    TOKEN_KEYWORD_VAR,          // var
    TOKEN_KEYWORD_LET,          // let
    TOKEN_KEYWORD_CONST,        // const
    TOKEN_KEYWORD_IF,           // if
    TOKEN_KEYWORD_ELSE,         // else
    TOKEN_KEYWORD_FOR,          // for
    TOKEN_KEYWORD_WHILE,        // while
    TOKEN_KEYWORD_TRUE,         // true
    TOKEN_KEYWORD_FALSE,        // false
    TOKEN_KEYWORD_RETURN,       // return
    TOKEN_KEYWORD_DO,           // do
    TOKEN_KEYWORD_CLASS,        // class
    TOKEN_KEYWORD_CONTINUE,     // continue
    TOKEN_KEYWORD_BREAK,        // break

    // Useless tokens / EOF
    TOKEN_SKIP,                 // Skip
    TOKEN_EOF,                  // End of file
} perf_e_token_type_t;

/**
 * We can use the token type as an index to get the token name as a string.
 **/
static const char* token_map[] = {
    "TOKEN_LEFT_PARENTHESES",
    "TOKEN_RIGHT_PARENTHESES",
    "TOKEN_LEFT_BRACE",
    "TOKEN_RIGHT_BRACE",
    "TOKEN_COMMA",
    "TOKEN_PERIOD",
    "TOKEN_SEMICOLON",
    "TOKEN_COLON",
    "TOKEN_MINUS",
    "TOKEN_PLUS",
    "TOKEN_SLASH",
    "TOKEN_ASTERISK",
    "TOKEN_PERCENT",
    "TOKEN_AMPERSAND",
    "TOKEN_EXCLAIM",
    "TOKEN_EXCLAIM_EQUAL",
    "TOKEN_EQUAL",
    "TOKEN_EQUAL_EQUAL",
    "TOKEN_GREATER",
    "TOKEN_GREATER_EQUAL",
    "TOKEN_LESS",
    "TOKEN_LESS_EQUAL",
    "TOKEN_IDENTIFIER",
    "TOKEN_STRING",
    "TOKEN_NUMBER",
    "TOKEN_INTEGER",
    "TOKEN_KEYWORD_FUNC",
    "TOKEN_KEYWORD_VAR",
    "TOKEN_KEYWORD_LET",
    "TOKEN_KEYWORD_CONST",
    "TOKEN_KEYWORD_IF",
    "TOKEN_KEYWORD_ELSE",
    "TOKEN_KEYWORD_FOR",
    "TOKEN_KEYWORD_WHILE",
    "TOKEN_KEYWORD_TRUE",
    "TOKEN_KEYWORD_FALSE",
    "TOKEN_KEYWORD_RETURN",
    "TOKEN_KEYWORD_DO",
    "TOKEN_KEYWORD_CLASS",
    "TOKEN_KEYWORD_CONTINUE",
    "TOKEN_KEYWORD_BREAK",
    "TOKEN_SKIP",
    "TOKEN_EOF"
};

/**
 * Represents a token parsed from src code.
 */
typedef struct _perf_token_t
{
    perf_e_token_type_t type;           // Type of token
    uint32_t            line_number;    // Line number of token
    uint32_t            column_number;  // Column number of token

    /**
     * The token's value. Can be a string, integer, or number.
     **/
    union _perf_token_repr_t
    {
        char        *str;       // String
        uint64_t    integer;    // Integer
        double      number;     // Number
    } as;
} perf_token_t;

/*
 * Frees the memory allocated for the token.
 * 
 * @param token The token to free.
 * 
 * @return PERF_RES_OK if successful, otherwise an error code.
 */
perf_result_t perf_token_free(perf_token_t *token); 

/*
 * Prints out all information about the token.
 *
 * @param token The token to print.
 *
 * @return PERF_RES_OK if successful, otherwise an error code.
 */
perf_result_t perf_token_print(perf_token_t *token);

#endif // _PERFECTION_TOKEN_H