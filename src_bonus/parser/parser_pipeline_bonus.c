/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:35:25 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/09 08:06:31 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "ast.h"
#include <stddef.h>

/**
 * @brief Check if a token type is a logical operator
 * 
 * @param type Token type to check
 * @return 1 if logical operator and 0 if not
 */
static int	is_logic_op(t_token_type type)
{
	return (type == TOKEN_AND || type == TOKEN_OR);
}

/**
 * @brief Get the logical operator (&& or ||) and map it to AST node type
 * 
 * @param type TOKEN_AND or TOKEN_OR
 * @return NODE_AND or NODE_OR
 */
static	t_node_type	get_logic_type(t_token_type type)
{
	if (type == TOKEN_AND)
		return (NODE_AND);
	return (NODE_OR);
}

/**
 * @brief Parse a list of pipelines connected by && or ||
 * 
 * Left associative: 
 * A && B || C ==> OR(AND(A, B), C)
 * 
 * @param tokens Pointer to current token pointer
 * @return AND/OR AST node or NULL on failure
 * @note This follows the same structure as parse_pipeline().
 * The only difference is the operator tokens and node types
 */
t_ast	*parse_and_or(t_token **tokens)
{
	t_ast		*left;
	t_ast		*right;
	t_node_type	ntype;

	left = parse_pipeline(tokens);
	if (!left)
		return (NULL);
	while (*tokens && is_logic_op((*tokens)->type))
	{
		ntype = get_logic_type((*tokens)->type);
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = create_and_or_node(ntype, left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}

/**
 * @brief Parse a pipeline of commands connected by pipes
 * 
 * Left associative tree: "A | B | C" -> PIPE(PIPE(A, B), C)
 * 
 * @param tokens Pointer to current token pointer
 * @return Pipeline AST or NULL on failure
 */
t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_command(tokens);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
		left = create_pipe_node(left, right);
		if (!left)
			return (NULL);
	}
	return (left);
}
