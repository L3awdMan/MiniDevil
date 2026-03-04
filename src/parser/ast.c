/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:42:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 04:46:02 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "libft.h"

/**
 * @brief Create a NODE_COMMAND AST node
 * 
 * @param args Argument array (NULL terminated)
 * @param argc Argument count
 * @return New command node or NULL on failure
 */
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

/**
 * @brief Create a pipe AST node connecting 2 subtrees
 * 
 * @param left Left child (command before the pipe)
 * @param right Right child (command after the pipe)
 * @return New pipe node or NULL on failure
 */
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

/**
 * @brief Create a redirection AST node
 * 
 * Initializes heredoc_fd to -1 (not used)
 * 
 * @param type Redirection type
 * @param file Filename or heredoc delimiter
 * @param cmd COmmand subtree that this redirection wraps
 * @param quoted 1 if heredoc delimiter was quoted
 * @return New redirection node or NULL on failure
 */
t_ast	*create_redir_node(t_node_type type, char *file, t_ast *cmd, int quoted)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
		return (NULL);
	node->type = type;
	node->data.redir.file = file;
	node->data.redir.cmd = cmd;
	node->data.redir.redir_type = type;
	node->data.redir.quote = quoted;
	node->data.redir.heredoc_fd = -1;
	return (node);
}
