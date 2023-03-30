#include "../inc/shared.h"
#include "../inc/result.h"
#include "../inc/token.h"
#include "../inc/lexer.h"
#include "../inc/ast.h"
#include "../inc/parser.h"

/**
 * Resize the AST nodes buffer.
 * 
 * @param out_ast_nodes The AST nodes buffer to resize.
 * @param capacity The capacity of the AST nodes buffer.
 * 
 * @return PERF_RES_OK if the AST nodes buffer was resized successfully.
 */
perf_result_t reallocate_ast_nodes_buffer(perf_parser_node_t **out_ast_nodes, int32_t *capacity)
{
	// Adjust the capacity of the AST nodes buffer.
	*capacity = ((*capacity) < 16) ? 16 : (*capacity) * 2;

	// Check if we need to malloc the AST nodes buffer.
	if (*out_ast_nodes == NULL)
	{
        // Allocate memory for the AST nodes buffer
		*out_ast_nodes = malloc(sizeof(perf_parser_node_t) * (*capacity));

		// Check if the AST nodes buffer was allocated successfully
        if (*out_ast_nodes == NULL)
        {
            // Return an error
            return PERF_RES_MEMORY_ALLOC_FAIL;
        }
	}

	// Otherwise, reallocate the existing memory block.
	else
	{
        // Resize the AST nodes buffer
		*out_ast_nodes = realloc(*out_ast_nodes, sizeof(perf_parser_node_t) * (*capacity));

		// Check if the AST nodes buffer was reallocated successfully
        if (out_ast_nodes == NULL)
        {
            // Return an error
            return PERF_RES_MEMORY_ALLOC_FAIL;
        }
	}

    // Return ok
	return PERF_RES_OK;
}


// Implementation for parser.h perf_parser_init
perf_result_t perf_parser_init(perf_parser_t *parser, perf_lexer_t *lexer, const char** error)
{
    // Check if the parser is null
    if (parser == NULL)
    {
        // Set the error message
        *error = "Parser is null";

        // Return an error
        return PERF_RES_PARSE_ERROR;
    }

    // Check if the lexer is null
    if (lexer == NULL)
    {
        // Set the error message
        *error = "Lexer is null";

        // Return an error
        return PERF_RES_PARSE_ERROR;
    }

    // Set the lexer
    parser->lexer = lexer;

    // Set the current token
    parser->current_token = NULL;

    // Set the erroring token
    parser->erroring_token = NULL;

    // Return ok
    return PERF_RES_OK;
}

/**
 * @brief Parse a primary expression.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_primary_expression(perf_parser_t* parser)
{
    perf_parser_result_t result;

	result.is_error = true;
	result.node = malloc(sizeof(perf_parser_node_t));

	if (parser->current_token->type == TOKEN_IDENTIFIER)
	{
		result.is_error = false;
		result.node->node_type = AST_VARIABLE;
		result.node->as.variable = parser->current_token++;
	}
	else if ((parser->current_token->type >= TOKEN_NUMBER && parser->current_token->type <= TOKEN_STRING)
		|| parser->current_token->type == TOKEN_KEYWORD_TRUE || parser->current_token->type == TOKEN_KEYWORD_FALSE)
	{
		result.is_error = false;
		result.node->node_type = AST_CONSTANT;
		result.node->as.constant = parser->current_token++;
	}
	else if (parser->current_token->type == TOKEN_LEFT_PARENTHESES)
	{
		parser->current_token++;

		if (parser->current_token->type == TOKEN_EOF)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			result.error = "expected identifier, number, integer or string literal";

			parser->erroring_token = parser->current_token;

			return result;
		}

		perf_parser_result_t nested_expr = perf_parser_parse_expression(parser);

		if (nested_expr.is_error)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			return nested_expr;
		}

		if ((parser->current_token++)->type != TOKEN_RIGHT_PARENTHESES)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			if (nested_expr.node != NULL)
			{
				perf_parser_node_free(nested_expr.node);
				free(nested_expr.node);
			}

			result.error = "expected closing parenthesis";

			parser->erroring_token = (parser->current_token - 1);

			return result;
		}

		result.is_error = false;
		result.node->node_type = AST_GROUP_EXPR;
		result.node->as.group_expression = nested_expr.node;
	}
	else
	{
		if (result.node != NULL)
		{
			perf_parser_node_free(result.node);
			free(result.node);
		}

		result.error = "unexpected token";

		parser->erroring_token = parser->current_token;
	}

	return result;
}

/**
 * @brief Parse a unary expression.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_unary_expression(perf_parser_t* parser)
{
    if (parser->current_token->type == TOKEN_EXCLAIM || parser->current_token->type == TOKEN_MINUS)
	{
		perf_parser_result_t result;

		result.is_error = false;
		result.node = malloc(sizeof(perf_parser_node_t));

		result.node->node_type = AST_UNARY_EXPR;
		result.node->as.unary_expr.op_token = parser->current_token++;

		perf_parser_result_t nested_expr = perf_parser_parse_unary_expression(parser);

		if (nested_expr.is_error)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			return nested_expr;
		}

		result.node->as.unary_expr.operand = nested_expr.node;

		return result;
	}

	return perf_parser_parse_primary_expression(parser);
}

/**
 * @brief Parse a multiplication expression.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_multiplication_expression(perf_parser_t* parser)
{
    perf_parser_result_t nested_lhs_expr = perf_parser_parse_unary_expression(parser);

	if (nested_lhs_expr.is_error)
		return nested_lhs_expr;

	while (parser->current_token->type == TOKEN_ASTERISK || parser->current_token->type == TOKEN_SLASH)
	{
		perf_parser_result_t result;

		result.is_error = false;
		result.node = malloc(sizeof(perf_parser_node_t));

		result.node->node_type = AST_BINARY_EXPR;
		result.node->as.binary_expr.op_token = parser->current_token++;

		perf_parser_result_t nested_rhs_expr = perf_parser_parse_unary_expression(parser);

		if (nested_rhs_expr.is_error)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			if (nested_lhs_expr.node != NULL)
			{
				perf_parser_node_free(nested_lhs_expr.node);
				free(nested_lhs_expr.node);
			}

			return nested_rhs_expr;
		}

		result.node->as.binary_expr.left = nested_lhs_expr.node;
		result.node->as.binary_expr.right = nested_rhs_expr.node;

		nested_lhs_expr = result;
	}

	return nested_lhs_expr;
}

/**
 * @brief Parse an addition expression.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_addition_expression(perf_parser_t* parser)
{
    perf_parser_result_t nested_lhs_expr = perf_parser_parse_multiplication_expression(parser);

	if (nested_lhs_expr.is_error)
		return nested_lhs_expr;

	while (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS)
	{
		perf_parser_result_t result;

		result.is_error = false;
		result.node = malloc(sizeof(perf_parser_node_t));

		result.node->node_type = AST_BINARY_EXPR;
		result.node->as.binary_expr.op_token = parser->current_token++;

		perf_parser_result_t nested_rhs_expr = perf_parser_parse_multiplication_expression(parser);

		if (nested_rhs_expr.is_error)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			if (nested_lhs_expr.node != NULL)
			{
				perf_parser_node_free(nested_lhs_expr.node);
				free(nested_lhs_expr.node);
			}

			return nested_rhs_expr;
		}

		result.node->as.binary_expr.left = nested_lhs_expr.node;
		result.node->as.binary_expr.right = nested_rhs_expr.node;

		nested_lhs_expr = result;
	}

	return nested_lhs_expr;
}

/**
 * @brief Parse a comparison expression.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_comparison_expression(perf_parser_t *parser)
{
    perf_parser_result_t nested_lhs_expr = perf_parser_parse_addition_expression(parser);

	if (nested_lhs_expr.is_error)
		return nested_lhs_expr;

	while (parser->current_token->type == TOKEN_GREATER || parser->current_token->type == TOKEN_GREATER_EQUAL
		|| parser->current_token->type == TOKEN_LESS || parser->current_token->type == TOKEN_LESS_EQUAL)
	{
		perf_parser_result_t result;

		result.is_error = false;
		result.node = malloc(sizeof(perf_parser_node_t));

		result.node->node_type = AST_BINARY_EXPR;
		result.node->as.binary_expr.op_token = parser->current_token++;

		perf_parser_result_t nested_rhs_expr = perf_parser_parse_addition_expression(parser);

		if (nested_rhs_expr.is_error)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			if (nested_lhs_expr.node != NULL)
			{
				perf_parser_node_free(nested_lhs_expr.node);
				free(nested_lhs_expr.node);
			}

			return nested_rhs_expr;
		}

		result.node->as.binary_expr.left = nested_lhs_expr.node;
		result.node->as.binary_expr.right = nested_rhs_expr.node;

		nested_lhs_expr = result;
	}

	return nested_lhs_expr;
}

/**
 * @brief Parse an equality expression.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_equality_expression(perf_parser_t *parser)
{
    // Parse as a comparison expression
    perf_parser_result_t nested_lhs_expr = perf_parser_parse_comparison_expression(parser);

	if (nested_lhs_expr.is_error)
		return nested_lhs_expr;

	while (parser->current_token->type == TOKEN_EXCLAIM_EQUAL || parser->current_token->type == TOKEN_EQUAL_EQUAL)
	{
		perf_parser_result_t result;

		result.is_error = false;
		result.node = malloc(sizeof(perf_parser_node_t));

		result.node->node_type = AST_BINARY_EXPR;
		result.node->as.binary_expr.op_token = parser->current_token++;

		perf_parser_result_t nested_rhs_expr = perf_parser_parse_comparison_expression(parser);

		if (nested_rhs_expr.is_error)
		{
			if (result.node != NULL)
			{
				perf_parser_node_free(result.node);
				free(result.node);
			}

			if (nested_lhs_expr.node != NULL)
			{
				perf_parser_node_free(nested_lhs_expr.node);
				free(nested_lhs_expr.node);
			}

			return nested_rhs_expr;
		}

		result.node->as.binary_expr.left = nested_lhs_expr.node;
		result.node->as.binary_expr.right = nested_rhs_expr.node;

		nested_lhs_expr = result;
	}

	return nested_lhs_expr;
}

/**
 * @brief Parse an expression.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_expression(perf_parser_t *parser)
{
    return perf_parser_parse_equality_expression(parser);
}

/**
 * @brief Parse a function definition.
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
*/
perf_parser_result_t perf_parser_parse_function_definition(perf_parser_t *parser)
{
    perf_parser_result_t result;
    result.is_error = true;
    result.error = "Not yet implemented";
    return result;
}

/**
 * @brief Parse generic statement
 * 
 * @param parser The parser to use.
 * 
 * @return PERF_PARSER_RESULT_T The result of the parse.
 */
perf_parser_result_t perf_parser_parse_generic_statement(perf_parser_t *parser)
{
    // Check for a function definition
    if (parser->current_token->type == TOKEN_KEYWORD_FUNC)
    {
        // Parse a function definition
        return perf_parser_parse_function_definition(parser);
    }

    // Otherwise, handle anything else.
    else
    {
        // Parse expression
        perf_parser_result_t expression = perf_parser_parse_expression(parser);

    }
}



// Implementation for parser.h perf_parser_parse
perf_result_t perf_parser_digest(perf_parser_t *parser, perf_token_t *tokens, int32_t token_count, perf_parser_node_t **ast, int32_t *ast_count, const char** error)
{

    // Used to keep track of how many nodes we have, and the maximum number of nodes we can have
    int32_t node_count = 0;
    int32_t max_node_count = 1;

    // Will store the AST nodes.
    perf_parser_node_t* nodes = NULL;

    // Allocate the ast nodes buffer
    perf_result_t result = reallocate_ast_nodes_buffer(&nodes, &max_node_count);

    // Check if the AST nodes buffer was allocated successfully
    if (result != PERF_RES_OK)
    {
        // Set the error message
        *error = "Failed to allocate AST nodes buffer";

        // Return an error
        return PERF_RES_MEMORY_ALLOC_FAIL;
    }

    // Set the output buffer & count
    *ast        = nodes;
    *ast_count  = node_count;

    // Set the current token to the first token available.
	parser->current_token   = &tokens[0];
	parser->erroring_token  = parser->current_token;

    // Loop until we reach the end of the token stream.
    while (parser->current_token->type != TOKEN_EOF)
    {
        // Check if we have enough space for nodes.
		if (node_count >= (max_node_count * 0.75))
		{
            // Allocate more memory for the AST nodes buffer
			perf_result_t result = reallocate_ast_nodes_buffer(&nodes, &max_node_count);
			
            // Check if the AST nodes buffer was allocated successfully
            if (result != PERF_RES_OK)
            {
                // Set the error message
                *error = "Failed to allocate AST nodes buffer";

                // Return an error
                return PERF_RES_MEMORY_ALLOC_FAIL;
            }

            // Set the output buffer & count
            *ast        = nodes;
            *ast_count  = node_count;
		}

        // Run the statement parser.
		perf_parser_result_t parse_result = perf_parser_parse_generic_statement(parser);

		// Return back to the caller if the parser function return an error.
		if (parse_result.is_error)
		{
			*error = parse_result.error;

			return PERF_RES_PARSE_ERROR;
		}

		// Copy the resulting AST node over and free the previous one.
		memcpy(&nodes[ node_count++ ], parse_result.node, sizeof(perf_parser_node_t));
		free(parse_result.node);
    }

    *ast        = nodes;
    *ast_count  = node_count;

    return PERF_RES_OK;
}