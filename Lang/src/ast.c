#include "../inc/shared.h"
#include "../inc/result.h"
#include "../inc/token.h"
#include "../inc/ast.h"

/**
 * @brief Frees a node and all of its children.
 * 
 * @param node The node to free.
 * 
 * @return PERF_RES_OK if the node was freed successfully.
*/
void perf_parse_node_free_internal( perf_parser_node_t *node, perf_parser_node_t *parent_node )
{
    // Check for expression nodes
	if ( node->node_type == AST_GROUP_EXPR )
	{
        // Free the group expression
		perf_parse_node_free_internal( node->as.group_expression, node );

        // Set the group expression to NULL
		node->as.group_expression = NULL;
	}

    // Check for unary expression nodes
	else if ( node->node_type == AST_UNARY_EXPR )
	{
        // Free the operand
		perf_parse_node_free_internal( node->as.unary_expr.operand, node );

        // Set the operand to NULL
		node->as.unary_expr.operand = NULL;
	}

    // Check for binary expression nodes
	else if ( node->node_type == AST_BINARY_EXPR )
	{
        // Free the left and right nodes
		perf_parse_node_free_internal( node->as.binary_expr.left, node );
		perf_parse_node_free_internal( node->as.binary_expr.right, node );

        // Set the left and right nodes to NULL
		node->as.binary_expr.left = NULL;
		node->as.binary_expr.right = NULL;
	}

    // Check for expression statement nodes
	else if ( node->node_type == AST_EXPR_STATEMENT )
	{
        // Free the expression statement
		perf_parse_node_free_internal( node->as.expr_statement, node );

        // Set the expression statement to NULL
		node->as.expr_statement = NULL;
	}

    // Check for constant nodes
	if ( parent_node != NULL )
		free( node );   // Free the node
}

// Implementation of ast.h perf_parser_node_free
perf_result_t perf_parser_node_free( perf_parser_node_t *node )
{
    // Free the node and all of its children
	perf_parse_node_free_internal( node, NULL );

    // Return PERF_RES_OK
	return PERF_RES_OK;
}