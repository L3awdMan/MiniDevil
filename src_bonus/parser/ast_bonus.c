/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 20:42:39 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/09 07:54:15 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "libft.h"

/**
 * @brief Create a subshell node
 * 
 * @param child Inner AST to execute inside the subshell
 * @return New subshell node or NULL if failed (frees child)
 */
t_ast	*create_subshell_node(t_ast *child)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
	{
		free_ast(child);
		return (NULL);
	}
	node->type = NODE_SUBSHELL;
	node->data.subshell.child = child;
	return (node);
}

/**
 * @brief Create a NODE_AND or NODE_OR binary AST node
 * 
 * @param type NODE_AND or NODE_OR
 * @param left LEft subtree
 * @param right Right subtree
 * @return New binary node or NULL if failed (frees both children)
 */
t_ast	*create_and_or_node(t_node_type type, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = ft_calloc(sizeof(t_ast), 1);
	if (!node)
	{
		free_ast(right);
		free_ast(left);
		return (NULL);
	}
	node->type = type;
	node->data.binary.left = left;
	node->data.binary.right = right;
	return (node);
}

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
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
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
 * @param cmd Command subtree that this redirection wraps
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
