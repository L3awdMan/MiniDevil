/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 21:48:21 by baelgadi          #+#    #+#             */
/*   Updated: 2026/03/04 04:47:36 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * @brief Check if an AST node is of redirection type
 * 
 * @param node Node to check
 * @return 1 if redirection and 0 if not
 */
static int	is_redir(t_ast *node)
{
	if (!node)
		return (0);
	return (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC);
}

/**
 * @brief Reverse a redirection chain for left to right execution order
 * 
 * The parser builds redirections with the outermost being last, so this
 * reverses the chain so that the first parsed redirection is outermost,
 * matching BASH's left to right behavior
 * 
 * @param node Outermost redirection
 * @return New outermost node of the reverse chain
 */
t_ast	*reverse_redir_chain(t_ast *node)
{
	t_ast	*prev;
	t_ast	*next;

	if (!is_redir(node))
		return (node);
	prev = node;
	while (is_redir(prev))
		prev = prev->data.redir.cmd;
	while (is_redir(node))
	{
		next = node->data.redir.cmd;
		node->data.redir.cmd = prev;
		prev = node;
		node = next;
	}
	return (prev);
}
