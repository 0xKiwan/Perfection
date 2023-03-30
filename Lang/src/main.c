#include "../inc/shared.h"
#include "../inc/result.h"
#include "../inc/token.h"
#include "../inc/lexer.h"


/**
 * Read the contents of a file.
 * 
 * @param path The path to the file.
 * 
 * @return The contents of the file.
*/
char* load_file(char const* path)
{
    // Will store the contents of the file
    char* buffer = 0;

    // Will store the length of the file
    long length;

    // Open handle to the file
    FILE * f = fopen (path, "rb");

    // Check if the file was opened successfully
    if (f)
    {
        // Get the length of the file
        fseek(f, 0, SEEK_END);

        // Store the length of the file
        length = ftell(f);

        // Reset the file pointer
        fseek (f, 0, SEEK_SET);

        // Allocate memory for the file
        buffer = (char*)malloc ((length+1)*sizeof(char));
        
        // Read the file into the buffer
        if (buffer) fread (buffer, sizeof(char), length, f);

        // Close the file
        fclose (f);
    }

    // Null terminate the buffer
    buffer[length] = '\0';

    // Return the buffer
    return buffer;
}


int main(int argc, char **argv) {

    // Create a lexer
    perf_lexer_t lexer;

    // Check if we should parse file or cli
    if ( argc > 1 ) 
    {
        // Read the file into a buffer
        char* buffer = load_file(argv[1]);

        // Will store the number of tokens in the file
        int32_t token_count = 0;

        // Will store the tokens
        perf_token_t *tokens = NULL;

        // Will store the error message
        const char *error = NULL;

        // Digest the file
        if (perf_lexer_digest( &lexer, buffer, &tokens, &token_count, &error) != PERF_RES_OK)
        {
            // Print the error
            printf("Error: %s\n", error);

            // Exit the program with an error
            return 1;
        }

        // Free the file contents buffer since we don't need it anymore
        free(buffer);
        
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