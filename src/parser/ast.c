/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:42:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/02/23 22:26:23 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "structs.h"

t_ast	*create_cmd_node(char **args, int argc)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
		return (NULL);
	node->type = NODE_COMMAND;
	node->data.cmd.args = args;
	node->data.cmd.argc = argc;
	return (node);
}

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
		return (NULL);
	node->type = NODE_PIPE;
	node->data.binary.left = left;
	node->data.binary.right = right;
	return (node);
}

t_ast	*create_redir_node(t_node_type type, char *file, t_ast *cmd, int quote)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->data.redir.file = file;
	node->data.redir.cmd = cmd;
	node->data.redir.redir_type = type;
	node->data.redir.quote = quote;
	node->data.redir.heredoc_fd = -1;
	return (node);
}
