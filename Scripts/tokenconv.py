

tokens = """
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
"""


for line in tokens.splitlines():
    tok = line.strip().split(",")[0]
    if not tok.startswith("TOKEN"): continue
    print("\"%s\"," % tok)