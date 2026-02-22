/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/21 21:48:21 by baelgadi          #+#    #+#             */
/*   Updated: 2026/02/22 01:08:27 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir(t_ast *node)
{
	if (!node)
		return (0);
	return (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC);
}

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