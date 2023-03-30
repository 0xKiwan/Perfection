#include "../inc/shared.h"
#include "../inc/result.h"
#include "../inc/token.h"
#include "../inc/lexer.h"




int main(int argc, char **argv) {

    // Create a lexer
    perf_lexer_t lexer;

    // Check if we should parse file or cli
    if ( argc > 1 ) 
    {
        // Open the file
        FILE *file = fopen( argv[ 1 ], "r" );

        // Check if the file was opened
        if ( file == NULL )
        {
            // Print error
            printf( "Could not open file '%s'.\n", argv[ 1 ] );

            // Exit
            return 1;
        }

        // Will store the file's contents
        char *file_contents = NULL;

        // Will store the file's size
        size_t file_size = 0;

        // Get the file's size
        fseek( file, 0, SEEK_END );
        file_size = ftell( file );
        rewind( file );

        // Allocate memory for the file's contents
        file_contents = ( char* )malloc( file_size + 1 );

        // Read the file's contents
        fread( file_contents, file_size, 1, file );

        // Null terminate the string
        file_contents[ file_size ] = '\x00';

        // Close the file
        fclose( file );

        // Will store the number of tokens in the file
        int32_t token_count = 0;

        // Will store the tokens
        perf_token_t *tokens = NULL;

        // Will store the error message
        const char *error = NULL;

        // Digest the file
        if (perf_lexer_digest( &lexer, file_contents, &tokens, &token_count, &error) != PERF_RES_OK)
        {
            // Print the error
            printf("Error: %s\n", error);

            // Exit the program with an error
            return 1;
        }

        // Free the file's contents
        free( file_contents );

        // Loop through the tokens
        for (int idx = 0; idx < token_count; idx++)
        {
            // Print the current token
            printf("Token %d/%d: %s\n", idx, token_count, token_map[tokens[idx].type]);
        }
    }

    // Otherwise parse command line input
    else
    {
        // Loop forever
        for (;;)
        {
            // Will store the user's input code.
            char line_buffer[ 1024 ];

            // Get the user's input
            fgets( line_buffer, sizeof( line_buffer ), stdin );

            // Break if the user entered a blank line
            if ( line_buffer[ 0 ] == '\n' ) return 1;

            // Null terminate the string
            line_buffer[ strlen( line_buffer ) - 1 ] = '\x00';

            // Will store the number of tokens in the file
            int32_t token_count = 0;

            // Will store the tokens
            perf_token_t *tokens = NULL;

            // Will store the error message
            const char *error = NULL;

            // Digest the line
            if (perf_lexer_digest( &lexer, line_buffer, &tokens, &token_count, &error) != PERF_RES_OK)
            {
                // Print the error
                printf("Error: %s\n", error);

                // Exit the program with an error
                return 1;
            }

            // Loop through the tokens
            for (int idx = 0; idx < token_count; idx++)
            {
                // Print the current token
                printf("Token %d/%d: %s\n", idx, token_count, token_map[tokens[idx].type]);
            }
        }
    }
}