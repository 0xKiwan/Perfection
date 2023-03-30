#ifndef _PERF_PARSER_H_
#define _PERF_PARSER_H_

/**
 * Represents our parser, which is used to parse the token stream into an AST.
 */
typedef struct _perf_parser_t
{
    perf_lexer_t* lexer;
    perf_token_t* current_token;
    perf_token_t* erroring_token;
} perf_parser_t;

/**
 * Represents a parser result.
*/
typedef struct _perf_parser_result_t
{
	bool is_error;  // True if the result is an error.

	union
	{
		perf_parser_node_t *node;   // The node that was parsed.
		const char* error;          // The error message
	};
} perf_parser_result_t;

perf_parser_result_t perf_parser_parse_primary_expression(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_unary_expression(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_multiplication_expression(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_addition_expression(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_comparison_expression(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_equality_expression(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_expression(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_function_definition(perf_parser_t *parser);
perf_parser_result_t perf_parser_parse_generic_statement(perf_parser_t *parser);


/**
 * @brief Initializes a parser.
 * 
 * @param parser The parser to initialize.
 * @param lexer The lexer to use.
 * @param error The error message to print if result is not RES_OK.
 * 
 * @return PERF_RES_OK if the parser was initialized successfully.
 */
perf_result_t perf_parser_init(perf_parser_t *parser, perf_lexer_t *lexer, const char** error);

/**
 * @brief Consume the token stream and parse it into an AST.
 * 
 * @param parser The parser to use.
 * @param tokens The token stream to parse.
 * @param token_count The number of tokens in the token stream.
 * @param ast The AST to populate.
 * @param ast_count The number of nodes in the AST.
 * @param error The error message to print if result is not RES_OK.
 * 
 * @return PERF_RES_OK if the token stream was parsed successfully.
 */
perf_result_t perf_parser_digest(perf_parser_t *parser, perf_token_t *tokens, int32_t token_count, perf_parser_node_t **ast, int32_t *ast_count, const char** error);


#endif // _PERF_PARSER_H_