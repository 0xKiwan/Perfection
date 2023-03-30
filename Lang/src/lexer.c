#include "../inc/shared.h"
#include "../inc/result.h"
#include "../inc/token.h"
#include "../inc/lexer.h"
#include "../inc/util.h"


// Implementation for lexer.h perf_lexer_init
perf_result_t perf_lexer_init(perf_lexer_t *lexer)
{
    // Initialize lexer fields
    lexer->token_start      = NULL;
    lexer->current_ch       = NULL;
    lexer->line_number      = 0;
    lexer->column_number    = 0;

    // Return OK result
    return PERF_RES_OK;
}

