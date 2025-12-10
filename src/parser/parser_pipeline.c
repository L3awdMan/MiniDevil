/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 22:35:25 by zotaj-di          #+#    #+#             */
/*   Updated: 2025/12/10 20:41:15 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "structs.h"

//==================== FUNCTION: parse_pipeline ==========================
	//
	// PURPOSE:
	//    Parse commands connected by pipes
	//    Handles: cmd1 | cmd2 | cmd3 ...
	//
	// RETURN:
	//    t_ast * - Pipe node tree or simple command, NULL on error
	//
	// PARAMETERS:
	//    t_token **tokens - Pointer to current token
	//
	// VARIABLES:
	//    t_ast *left  - Left side of pipe (accumulator)
	//    t_ast *right - Right side of current pipe
	//
	// ALGORITHM:
	//    1. Parse first command (becomes left)
	//    2. While current token is PIPE:
	//       a. Skip the pipe token
	//       b. Parse next command (becomes right)
	//       c. Create pipe node: PIPE(left, right)
	//       d. New pipe becomes left for next iteration
	//    3. Return final tree
	//
	// EXAMPLE:
	//    Input: cat | grep | sort
	//    Step 1: left = CMD(cat)
	//    Step 2: left = PIPE(CMD(cat), CMD(grep))
	//    Step 3: left = PIPE(PIPE(cat, grep), CMD(sort))
	//    Result: Tree with cat at leftmost leaf

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;

	left = parse_simple_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_simple_command(tokens);
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
