/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_subshell_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 06:33:13 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/09 06:42:55 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "ast.h"
#include "libft.h"

/**
 * @brief Parse a subshell expression
 * 
 * Advances after the opening `(` token and parses the inner command list using
 * parse_and_or() while expecting a closing `)` token
 * 
 * @param tokens Pointer to current token pointer
 * @return Subshell AST Node or NULL on error
 */
t_ast	*parse_subshell(t_token **tokens)
{
	t_ast	*inside;

	*tokens = (*tokens)->next;
	inside = parse_and_or(tokens);
	if (!inside)
		return (NULL);
	if (!(*tokens) || (*tokens)->type != TOKEN_RPAREN)
	{
		free_ast(inside);
		return (NULL);
	}
	*tokens = (*tokens)->next;
	return (create_subshell_node(inside));
}
