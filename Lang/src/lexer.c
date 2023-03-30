#include "../inc/shared.h"
#include "../inc/result.h"
#include "../inc/token.h"
#include "../inc/lexer.h"

/**
 * Used to determine the integer mode of an int literal.
*/
typedef enum _perf_e_integer_mode_t
{
    INT_BINARY  = 2,
    INT_OCTAL   = 8,
    INT_DECIMAL = 10,
    INT_HEX     = 16
} perf_e_integer_mode_t;

/**
 * @brief Determines if the given character is whitespace.
 * 
 * @param ch The character to check.
 * 
 * @return true if the character is whitespace, false otherwise.
*/
bool char_is_whitespace(char ch) 
{
    return ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n';
}

/**
 * @brief Determines if the given character is alphabetic.
 * 
 * @param ch The character to check.
 * 
 * @return true if the character is alphabetic, false otherwise.
*/
bool char_is_alphabetic(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

/**
 * @brief Determines if the given character is numeric.
 * 
 * @param ch The character to check.
 * 
 * @return true if the character is numeric, false otherwise.
*/
bool char_is_numeric(char ch)
{
	return ch >= '0' && ch <= '9';
}

/**
 * @brief Determines if the given character is alphanumeric.
 * 
 * @param ch The character to check.
 * 
 * @return true if the character is alphanumeric, false otherwise.
*/
bool char_is_alphanumeric(char ch)
{
    return char_is_alphabetic(ch) || char_is_numeric(ch);
}

/**
 * @brief Determines if the given character matches the given integer mode.
 * 
 * @param ch The character to check.
 * @param integer_mode The integer mode to check against.
 * 
 * @return true if the character matches the integer mode, false otherwise.
*/
bool char_matches_integer_mode(char ch, perf_e_integer_mode_t integer_mode)
{
    // Check if the character matches the integer mode.
	switch (integer_mode)
	{
	case INT_BINARY:    return ch == '0' || ch == '1';
	case INT_OCTAL:     return ch >= '0' && ch <= '8';
	case INT_DECIMAL:   return ch >= '0' && ch <= '9';
	case INT_HEX:       return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
	}

    // Return false if the integer mode is invalid.
	return false;
}

/**
 * @brief Resizes the token array.
 * 
 * @param token_array The token array to resize.
 * @param new_size The new size of the token array.
 * 
 * @return PERF_RES_OK if the token array was resized successfully.
*/
perf_result_t token_array_resize(perf_token_t** out, int32_t* capacity)
{
    // Adjust the capacity of the token buffer.
	*capacity = ((*capacity) < 16) ? 16 : (*capacity) * 2;

    // Check if the token buffer is NULL.
    if (*out == NULL)
    {
        // Allocate memory for the token buffer.
        *out = (perf_token_t*)malloc(sizeof(perf_token_t) * (*capacity));

        // Check if the memory allocation failed.
        if (*out == NULL)
        {
            // Return memory allocation failure result.
            return PERF_RES_MEMORY_ALLOC_FAIL;
        }
    }
    else
    {
        // Reallocate memory for the token buffer.
        *out = (perf_token_t*)realloc(*out, sizeof(perf_token_t) * (*capacity));

        // Check if the memory allocation failed.
        if (*out == NULL)
        {
            // Return memory allocation failure result.
            return PERF_RES_MEMORY_ALLOC_FAIL;
        }
    }

    // Return OK result.
    return PERF_RES_OK;
}


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

/**
 * @brief Handles any comments in the source code.
 * 
 * @param lexer The lexer to use.
 * @param is_single_line_comment Determines if it's a single line comment or a multi-line comment.
 * 
 * @return PERF_RES_OK if the comment was handled successfully.
*/
perf_result_t perf_lexer_handle_comment(perf_lexer_t *lexer, bool ds_comment)
{
    // Used to determine if the comment is terminated or not.
    bool is_terminated = ds_comment;

    // Loop until we reach either EOF or end of comment.
    while (*lexer->current_ch != '\x00')
    {
        // Get the current character.
        char ch = *lexer->current_ch;

        // Move to the next character.
        lexer->current_ch++;
        lexer->column_number++;

        // Check for a newline character
        if (ch == '\n')
        {
            lexer->line_number++;       // Increment line number
            lexer->column_number = 0;   // Reset to the start of the newline.
        }

        // Check if it's a multi-line comment. If not, then we can return.
        if (ds_comment && ch == '\n') break;

        // Check for comment termination
        if (!ds_comment && ch == '*' && *lexer->current_ch == '/')
        {
            // Move to the next character.
            lexer->current_ch++;
            lexer->column_number++;

            // Set the is_terminated flag to true.
            is_terminated = true;

            // Break out of the loop.
            break;
        }
    }
    

    // If comment is unterminated out of the loop, we have an error.
    if (!is_terminated)
    {
        // Print the error message.
        

        // Return the error result.
        return PERF_RES_LEX_ERROR;
    }

    // Return OK result.
    return PERF_RES_OK;
}

/**
 * @brief Handles any identifiers in the source code.
 * 
 * @param lexer The lexer to use.
 * @param token The token to use.
 * 
 * @return PERF_RES_OK if the identifier was handled successfully.
*/
perf_result_t perf_lexer_handle_identifier(perf_lexer_t* lexer, perf_token_t* token)
{
    // Loop until we reach the end of the identifier, or EOF.
    while (*lexer->current_ch != '\x00' && char_is_alphanumeric(*lexer->current_ch))
	{
        // Move to the next character.
		lexer->column_number++;
		lexer->current_ch++;
	}

    // Calculate the length of the identifier
    int32_t length = (int32_t)(lexer->current_ch - lexer->token_start);

    // Construct the token
    token->type             = TOKEN_IDENTIFIER;
    token->line_number      = lexer->line_number;
    token->column_number    = lexer->column_number - (int32_t)length;
    token->as.str           = malloc(length + 1);

    // Ensure the string was allocated successfully.
    if (token->as.str == NULL) return PERF_RES_MEMORY_ALLOC_FAIL;

    // Zero the allocated memory.
    memset(token->as.str, 0, length + 1);

    // Copy the identifier into the token.
    memcpy(token->as.str, lexer->token_start, length);

    // Handle keywords
    // TODO: This is a temporary solution. We should use a hash table instead.
    for (int idx = 26; idx <= 40; idx++)
    {
        // Find the matching keyword
        const char* keyword = keyword_map[idx - 26];
        
        // Find the token type
        const char* token_type = token_map[idx];

        // Now compare the keyword to the token representation
        if (strcmp(token->as.str, keyword) == 0)
        {
            // Set the token type
            token->type = (perf_e_token_type_t)idx;

            // Free the string
            free(token->as.str);

            // Return OK result
            return PERF_RES_OK;
        }
    }

    // Return OK result.
    return PERF_RES_OK;
}

/**
 * @brief Handles any numbers in the source code.
 * 
 * @param lexer The lexer to use.
 * @param token The token to use.
 * 
 * @return PERF_RES_OK if the number was handled successfully.
*/
perf_result_t perf_lexer_handle_number(perf_lexer_t *lexer, perf_token_t* token)
{
    // Used to determine if the number is a float or not
    bool is_float = false;

    // Used to determine the integer mode of the number.
    perf_e_integer_mode_t integer_mode = INT_DECIMAL;

    // Loop until we reach the end of the number, or EOF.
    while (*lexer->current_ch != '\x00')
    {
        // Handle digit seperators
        if (*lexer->current_ch == '_')
        {
            // Move to the next character.
            lexer->current_ch++;
            lexer->column_number++;

            // Continue to the next iteration.
            continue;
        }

        // Check we are at the start of the token
        if (lexer->current_ch == lexer->token_start)
        {
            // Check first for a single zero. This fixes an issue with defining zero numbers with our integer mode check.
            if (*lexer->current_ch == '0' && (*(lexer->current_ch + 1) == ' ' || *(lexer->current_ch + 1) == ';'))
            {
                continue;
            }

            // Handle integer mode notation
            if (*lexer->current_ch == '0' && *(lexer->current_ch + 1) != '\x00'
				&& *(lexer->current_ch + 1) != '.' && *(lexer->current_ch + 1) != '_'
				&& !char_is_numeric(*(lexer->current_ch + 1)))
			{
                // Move to the next character.
                lexer->current_ch++;
                lexer->column_number++;

                // Handle notations
                switch (*lexer->current_ch)
                {
                case 'b':                           // Handle binary notation
					lexer->column_number++;
					lexer->current_ch++;

					integer_mode = INT_BINARY;
					break;
				case 'o':                           // Handle octal notation   
					lexer->column_number++;
					lexer->current_ch++;

					integer_mode = INT_OCTAL;
					break;
				case 'x':                           // Handle hexadecimal notation
					lexer->column_number++;
					lexer->current_ch++;

					integer_mode = INT_HEX;
					break;
				default:                            // Handle unknown notation
					return PERF_RES_LEX_ERROR;
                }

                // Handle incomplete integer literal
                if (*lexer->current_ch == '\x00' || !char_matches_integer_mode(*lexer->current_ch, integer_mode)) return PERF_RES_LEX_ERROR;
            }

            // Check if next digit matches the integer mode
            else if (char_matches_integer_mode(*lexer->current_ch, integer_mode))
			{
                // Move to the next character.
				lexer->column_number++;
				lexer->current_ch++;
			}

            // Otherwise we have reached the end of the number.
            else break;
        }

        // Check if there's more digits to parse.
        else if (char_matches_integer_mode(*lexer->current_ch, integer_mode))
        {
            // Move to the next character.
            lexer->column_number++;
            lexer->current_ch++;
        }

        // Check for a floating point number.
        else if (integer_mode == INT_DECIMAL && !is_float && *lexer->current_ch == '.')
        {
            // Move to the next character.
            lexer->column_number++;
			lexer->current_ch++;

            // Mark the number as a float.
			is_float = true;
        }

        // Otherwise, we have reached the end of the number.
        else break;
    }

    // Calculate the length of the number.
    size_t literal_length = (size_t)(lexer->current_ch - lexer->token_start);

    // Check if we should skip integer notation
    size_t str_literal_offset = integer_mode == INT_DECIMAL ? 0 : 2;

    // Calculate the size of the string literal without the integer notation.
	size_t literal_str_size = (size_t)literal_length - str_literal_offset;

    // Will store the size of the literal without underscores.
	size_t size_without_uss = literal_str_size;

    // Check for a valid floating point number
    if (is_float && literal_str_size == 2) return PERF_RES_LEX_ERROR;

    // Loop through the string
    for ( size_t i = 0; i < literal_str_size; i++ )
	{
        // Check for an underscore, and if so, decrement the size.
		if (*(lexer->token_start + i + str_literal_offset) == '_') size_without_uss--;
	}

    // Allocate space for the raw integer literal
    char *literal_str = malloc( size_without_uss + 1 );

    // Check if the allocation failed.
    if (literal_str == NULL) return PERF_RES_MEMORY_ALLOC_FAIL;

    // Zero the memory.
    memset(literal_str, 0, size_without_uss + 1);

    // Loop through the string
    for ( size_t i = 0, j = 0; i < literal_str_size; i++ )
    {
        // If the char is not an underscore, we can store it.
        if (*(lexer->token_start + i + str_literal_offset) != '_')
        {
            // Store the character.
            *(literal_str + (j++)) = *(lexer->token_start + i + str_literal_offset);
        }
    }

    // Construct the token.
    token->type             = is_float ? TOKEN_NUMBER : TOKEN_INTEGER;
    token->line_number      = lexer->line_number;
    token->column_number    = lexer->column_number;

    // Check if the number is a float, and parse it.
    if (is_float) token->as.number = strtod(literal_str, NULL);

    // Otherwise, parse it as an integer.
    else token->as.integer = strtoull(literal_str, NULL, (int)integer_mode);

    // Return OK result.
    return PERF_RES_OK;
}

/**
 * @brief Handles any strings in the source code.
 * 
 * @param lexer The lexer to use.
 * @param token The token to use.
 * 
 * @return PERF_RES_OK if the string was handled successfully.
*/
perf_result_t perf_lexer_handle_string(perf_lexer_t* lexer, perf_token_t* token)
{
    // Skip the first quote
    lexer->current_ch++;
    lexer->column_number++;

    // Used to determine if the string is terminated or not.
    bool is_terminated = false;

    // Loop until we reach either EOF or end of string.
    while (*lexer->current_ch != '\x00' && !is_terminated)
	{
        // Check for escape character
        if (*lexer->current_ch == '\\')
		{
            // Move to the next character.
			lexer->current_ch++;
			lexer->column_number++;
		}

        // Check if the string is terminated
		else if (*lexer->current_ch == '"')
		{
            // Mark the string as terminated.
			is_terminated = true;
		}

        // Skip the current character
        lexer->current_ch++;
        lexer->column_number++;
    }

    // If string is unterminated out of the loop, we have an error.
    if (!is_terminated)
    {
        // Return the error result.
        return PERF_RES_LEX_ERROR;
    }

    // Calculate the length of the string ignoring the quotes.
    int32_t length = (int32_t)(lexer->current_ch - lexer->token_start) - 2;

    // Construct the token
    token->type             = TOKEN_STRING;
    token->line_number      = lexer->line_number;
    token->column_number    = lexer->column_number - (int32_t)(length + 2);
    token->as.str           = malloc(length + 1);

    // Ensure the string was allocated successfully.
    if (token->as.str == NULL) return PERF_RES_MEMORY_ALLOC_FAIL;

    // Zero the allocated memory.
    memset(token->as.str, 0, length + 1);

    // Copy the string into the token.
    memcpy(token->as.str, lexer->token_start + 1, length);

    // Return OK result.
    return PERF_RES_OK;
}

// Implementation for lexer.h perf_lexer_digest
perf_result_t perf_lexer_digest(perf_lexer_t *lexer, const char* src, perf_token_t **tokens, int32_t *token_count)
{
    // Save ptr to the start of the source code
    lexer->src = src;

    // Used to keep track of how many tokens there are, and how many we can allocate.
    int32_t num_tokens     = 0;
    int32_t token_capacity  = 1;

    // Initialize the token array.
    perf_token_t *token_array = NULL;

    // Allocate the token array
    perf_result_t result = token_array_resize(&token_array, &token_capacity);

    // Check if the token array was resized successfully.
    if (result != PERF_RES_OK)
    {
        // Return the result.
        return result;
    }

    // Update our output buffer & token count ptrs with the token array.
    *tokens = token_array;
    *token_count = num_tokens;

    // Set up the 'current_ch' pointer to the start of the source code.
    lexer->current_ch = src;

    // Loop until we reach EOF character.
    while (*lexer->current_ch != '\x00')
    {
        // Get the current character.
        char ch = *lexer->current_ch;

        // Check if the current character is whitespace.
        if (char_is_whitespace(ch))
        {
            // Skip to the next char & column.
            lexer->current_ch++;
            lexer->column_number++;

            // Check if it is a newline character.
            if (ch == '\n')
            {
                lexer->line_number++;       // Increment line number
                lexer->column_number = 0;   // Reset to the start of the newline.
            }
        }

        // Check for a comment
        else if (ch == '/' && (*(lexer->current_ch + 1) == '/' || *(lexer->current_ch + 1) == '*'))
        {
            // Used to determine if it's a single line comment or a multi-line comment.
            bool ds_comment = *(lexer->current_ch + 1) == '/';

            // Skip the comment characters.
            perf_result_t res = perf_lexer_handle_comment(lexer, ds_comment);

            // Check if the comment was handled successfully.
            if (res != PERF_RES_OK) return res;
        }

        // Otherwise, we can actually start lexing.
        else
        {
            // Save the start of the token.
            lexer->token_start = lexer->current_ch;

            // Check that we have enough space for the token.
            if (num_tokens >= (token_capacity * 0.75))
            {
                // Resize the token array.
                result = token_array_resize(&token_array, &token_capacity);

                // Check if the token array was resized successfully.
                if (result != PERF_RES_OK)
                {
                    // Return the result.
                    return result;
                }

                *tokens = token_array;
                *token_count = num_tokens;
            }

            // Get a pointer to the next token
            perf_token_t *token = &token_array[num_tokens++];

            // Handle an identifier
            if (char_is_alphabetic(ch))
            {
                // Handle the identifier
                perf_result_t parse_result = perf_lexer_handle_identifier(lexer, token);

                // Check if the identifier was handled successfully.
				if (parse_result != PERF_RES_OK) return parse_result;
            }

            // Handle a number
            else if (char_is_numeric(ch))
            {
                // Handle the number
                perf_result_t parse_result = perf_lexer_handle_number(lexer, token);

                // Check if the number was handled successfully.
                if (parse_result != PERF_RES_OK) return parse_result;
            }

            // Handle a string
            else if (ch == '"')
            {
                // Handle the string
                perf_result_t parse_result = perf_lexer_handle_string(lexer, token);

                // Check if the string was handled successfully.
                if (parse_result != PERF_RES_OK) return parse_result;
            }

            // Handle everything else
            else
            {
                switch (ch)
                {

                }
            }

        }
    }

    // Return OK result
    return PERF_RES_OK;
}