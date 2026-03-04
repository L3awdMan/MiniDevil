/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/04 07:00:50 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "token.h"
#include "libft.h"

/**
 * @brief Execute a NODE_COMMAND AST node
 * 
 * @param node AST node of type NODE_COMMAND
 * @param shell Shell context
 * @return Exit status of the executed command
 */
static int	exec_command_node(t_ast *node, t_shell *shell)
{
	char	**args;

	args = node->data.cmd.args;
	return (exec_simple_command(args, shell));
}

/**
 * @brief Main AST executor that dispatches by node type
 * 
 * - Recursively walks the AST and dispatches NODE_COMMAND, NODE_PIPE and
 * NODE_REDIR to exec_command_node(), handle_pipe() and handle_redir()
 * 
 * @param node Root of the AST subtree to execute
 * @param shell Shell context
 * @return Exit status of the executed pipe/command and 0 for NULL nodes
 */
int	executor(t_ast *node, t_shell *shell)
{
	if (!node)
		return (0);
	if (node->type == NODE_COMMAND)
		return (exec_command_node(node, shell));
	if (node->type == NODE_PIPE)
		return (handle_pipe(node, shell));
	if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
		return (handle_redir(node, shell));
	return (0);
}
