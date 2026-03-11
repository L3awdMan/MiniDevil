/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baelgadi <baelgadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:48:52 by zotaj-di          #+#    #+#             */
/*   Updated: 2026/03/06 07:39:59 by baelgadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "token.h"
#include "libft.h"

/**
 * @brief Restore the masked dollars in a string (after deferred expansion)
 * 
 * @param str String to unmask
 */
static void	unmask_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == MASK_DOLLAR)
			str[i] = '$';
		i++;
	}
}

/**
 * @brief Expand variables in an argument at execution time
 * 
 * @param arg Argument string to expand
 * @param sh Shell context
 * @return Expanded string (newly allocated) or original argument on failure
 */
static char	*expand_arg(char *arg, t_shell *sh)
{
	char	*expanded;

	expanded = expand_variables(arg, sh->env, QUOTE_NONE, sh->exit_status);
	if (!expanded)
		return (arg);
	unmask_str(expanded);
	return (expanded);
}

/**
 * @brief Expand the filename of a redirection node
 * 
 * Heredoc are skipped since their delimiter has to stay litteral
 * 
 * @param node Redirection AST node
 * @param shell Shell context
 */
static void	expand_redir_file(t_ast *node, t_shell *shell)
{
	char	*new_file;

	if (node->type == NODE_REDIR_HEREDOC)
		return ;
	new_file = expand_arg(node->data.redir.file, shell);
	if (new_file != node->data.redir.file)
	{
		free(node->data.redir.file);
		node->data.redir.file = new_file;
	}
}

/**
 * @brief Execute a NODE_COMMAND AST node
 * 
 * @param node AST node of type NODE_COMMAND
 * @param shell Shell context
 * @return Exit status of the executed command
 */
static int	exec_command_node(t_ast *node, t_shell *shell)
{
	int		i;
	char	*new_value;

	i = 0;
	while (node->data.cmd.args[i])
	{
		new_value = expand_arg(node->data.cmd.args[i], shell);
		if (new_value != node->data.cmd.args[i])
		{
			free(node->data.cmd.args[i]);
			node->data.cmd.args[i] = new_value;
		}
		i++;
	}
	return (exec_simple_command(node->data.cmd.args, shell));
}

/**
 * @brief Main AST executor that dispatches by node type
 * 
 * - Recursively walks the AST and dispatches NODE_COMMAND, NODE_PIPE, NODE_AND,
 * NODE_OR and NODE_REDIR to exec_command_node(), handle_pipe(), handle_and_or()
 * , handle_subshell() and handle_redir()
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
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (handle_and_or(node, shell));
	if (node->type == NODE_SUBSHELL)
		return (handle_subshell(node, shell));
	if (node->type >= NODE_REDIR_IN && node->type <= NODE_REDIR_HEREDOC)
	{
		expand_redir_file(node, shell);
		return (handle_redir(node, shell));
	}
	return (0);
}
