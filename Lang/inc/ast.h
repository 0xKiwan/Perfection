#ifndef _PERF_AST_H_
#define _PERF_AST_H_

/**
 * Used to determine which type of node is being used.
*/
typedef enum _perf_e_parser_node_type_t
{
	AST_CONSTANT,
	AST_VARIABLE,
	AST_GROUP_EXPR,
	AST_UNARY_EXPR,
	AST_BINARY_EXPR,
	AST_EXPR_FUNCTION_DEF,
	AST_EXPR_STATEMENT
} perf_e_parser_node_type_t;

/**
 * Represents a node in the abstract syntax tree.
*/
typedef struct _perf_parser_node_t
{
	perf_e_parser_node_type_t node_type;      // The type of node this is.

	union
	{
		perf_token_t *constant;
		perf_token_t *variable;

		struct _perf_parser_node_t *group_expression;
		struct _unary_expr_t
		{
			struct _perf_parser_node_t *operand;
			perf_token_t *op_token;
		} unary_expr;
		struct _binary_expr_t
		{
			struct _perf_parser_node_t *left;
			struct _perf_parser_node_t *right;
			perf_token_t *op_token;
		} binary_expr;
		struct _perf_parser_node_t *expr_statement;
	} as;
} perf_parser_node_t;

/**
 * @brief Frees a node in the AST.
 * 
 * @param node The node to free.
 * 
 * @return PERF_RES_OK if the node was freed successfully.
*/
perf_result_t perf_parser_node_free(perf_parser_node_t *node);

#endif // _PERF_AST_H_