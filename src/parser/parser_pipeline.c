/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:35:25 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/03 07:48:33 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

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
