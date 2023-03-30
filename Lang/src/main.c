#include "../inc/shared.h"
#include "../inc/result.h"
#include "../inc/token.h"
#include "../inc/lexer.h"


int main(int argc, char **argv) {


    perf_lexer_t lexer;

    for ( ;; )
	{
		char line_buffer[ 1024 ];
        fgets( line_buffer, sizeof( line_buffer ), stdin );
        if ( line_buffer[ 0 ] == '\n' )
			break;
        line_buffer[ strlen( line_buffer ) - 1 ] = '\x00';
        int32_t token_count = 0;

		perf_token_t *tokens = NULL;
		// perf_lexer_error_t lex_error = NULL;
        perf_lexer_digest( &lexer, line_buffer, &tokens, &token_count);
    }
    
}